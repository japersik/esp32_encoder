#include "japersik/esp32_encoder/pcnt_encoder.h"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "driver/pulse_cnt.h"
#include "freertos/FreeRTOS.h"
#include "freertos/projdefs.h"
#include "freertos/queue.h"
#include "hal/gpio_types.h"
#include "japersik/esp32_encoder/callback_funcs.h"
#include "portmacro.h"

struct PcntEncoder {
  EncoderCallbackFunc event_callback;
  void *event_callback_cxt;

  gpio_port_t a;
  gpio_port_t b;
  uint32_t glitch_filter_ns;
  int8_t downscale;
  bool inverse;

  QueueHandle_t processingQueue;
  int32_t position;
};

static bool _pcnt_on_reach(pcnt_unit_handle_t unit, const pcnt_watch_event_data_t *edata, void *pcnt_encoder) {
  BaseType_t high_task_wakeup;
  PcntEncoder *encoder = (PcntEncoder *)pcnt_encoder;
  xQueueSendFromISR(encoder->processingQueue, &(edata->watch_point_value), &high_task_wakeup);
  return (high_task_wakeup == pdTRUE);
}

void _monitor_pcnt_encoder_task(void *pcnt_encoder) {
  PcntEncoder *self = (PcntEncoder *)pcnt_encoder;
  int32_t event_count = 0;
  for (;;) {
    if (xQueueReceive(self->processingQueue, &event_count, portMAX_DELAY)) {
      self->position += event_count;
      EncoderCallbackEvent event = {.delta = event_count / self->downscale,
				    .position = self->position / self->downscale};
      if (self->event_callback) {
	self->event_callback(event, self->event_callback_cxt);
      }
    }
  }
}

void _init_encoder(PcntEncoder *self) {
  pcnt_unit_config_t unit_config = {
      .high_limit = self->downscale,
      .low_limit = -self->downscale,
  };

  pcnt_unit_handle_t pcnt_unit = NULL;
  pcnt_new_unit(&unit_config, &pcnt_unit);

  // setup glitch filter
  pcnt_glitch_filter_config_t filter_config = {
      .max_glitch_ns = self->glitch_filter_ns,
  };
  pcnt_unit_set_glitch_filter(pcnt_unit, &filter_config);

  // setup ports
  pcnt_chan_config_t chan_a_config = {
      .edge_gpio_num = self->a,
      .level_gpio_num = self->b,
  };
  pcnt_channel_handle_t pcnt_chan_a = NULL;
  pcnt_new_channel(pcnt_unit, &chan_a_config, &pcnt_chan_a);
  pcnt_chan_config_t chan_b_config = {
      .edge_gpio_num = self->b,
      .level_gpio_num = self->a,
  };
  pcnt_channel_handle_t pcnt_chan_b = NULL;
  pcnt_new_channel(pcnt_unit, &chan_b_config, &pcnt_chan_b);

  pcnt_channel_set_edge_action(pcnt_chan_a, PCNT_CHANNEL_EDGE_ACTION_DECREASE, PCNT_CHANNEL_EDGE_ACTION_INCREASE);
  pcnt_channel_set_level_action(pcnt_chan_a, PCNT_CHANNEL_LEVEL_ACTION_KEEP, PCNT_CHANNEL_LEVEL_ACTION_INVERSE);
  pcnt_channel_set_edge_action(pcnt_chan_b, PCNT_CHANNEL_EDGE_ACTION_INCREASE, PCNT_CHANNEL_EDGE_ACTION_DECREASE);
  pcnt_channel_set_level_action(pcnt_chan_b, PCNT_CHANNEL_LEVEL_ACTION_KEEP, PCNT_CHANNEL_LEVEL_ACTION_INVERSE);

  int watch_points[] = {-self->downscale, self->downscale};
  for (size_t i = 0; i < sizeof(watch_points) / sizeof(watch_points[0]); i++) {
    pcnt_unit_add_watch_point(pcnt_unit, watch_points[i]);
  }

  pcnt_event_callbacks_t cbs = {
      .on_reach = _pcnt_on_reach,
  };

  pcnt_unit_register_event_callbacks(pcnt_unit, &cbs, self);

  pcnt_unit_enable(pcnt_unit);
  pcnt_unit_clear_count(pcnt_unit);
  pcnt_unit_start(pcnt_unit);

  xTaskCreate(_monitor_pcnt_encoder_task, "monitor_pcnt_encoder_task", 2048, self, 1, NULL);
}

PcntEncoder *pcnt_encoder_new(gpio_port_t a, gpio_port_t b) {
  PcntEncoder *self = (PcntEncoder *)calloc(1, sizeof(PcntEncoder));
  if (self == NULL) {
    return NULL;
  }
  self->a = a;
  self->b = b;
  self->processingQueue = xQueueCreate(10, sizeof(int));
  self->glitch_filter_ns = 1000;
  self->downscale = 4;
  _init_encoder(self);
  return self;
}

void pcnt_encoder_free(PcntEncoder *self) { free(self); }

void pcnt_encoder_set_callback(PcntEncoder *self, EncoderCallbackFunc f, void *user_ctx) {
  self->event_callback = f;
  self->event_callback_cxt = user_ctx;
}

