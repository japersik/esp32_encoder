#ifndef _JAPERSIK_ESP32_ENCODER_VIRT_ENCODER_H
#define _JAPERSIK_ESP32_ENCODER_VIRT_ENCODER_H

#include <stdbool.h>
#include <stdint.h>

typedef void (*EncoderCallbackFunction)(uint8_t id, int64_t position);

typedef struct {
  uint8_t id;
  EncoderCallbackFunction event_callback;
  int64_t curruent_position;
  bool inverse;
  bool last_state_a;
  bool last_state_b;
  uint32_t last_event_time_ms;
} VirtEncoder;

VirtEncoder *encoder_new(uint8_t id);

void encoder_free(VirtEncoder *);
void encoder_set_callback(VirtEncoder *, EncoderCallbackFunction);
void encoder_rorate_on_time(VirtEncoder *, bool a_state, bool b_state, uint32_t time_ms);
#endif	// !_JAPERSIK_ESP32_ENCODER_VIRT_ENCODER_H
