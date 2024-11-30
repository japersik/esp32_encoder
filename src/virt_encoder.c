#include "japersik/esp32_encoder/virt_encoder.h"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

VirtEncoder *encoder_new(uint8_t id) {
  VirtEncoder *self = (VirtEncoder *)malloc(sizeof(VirtEncoder));
  if (self == NULL) {
    return NULL;
  }
  self->id = id;
  self->event_callback = NULL;
  self->curruent_position = 0;
  self->last_state_a = 0;
  self->last_state_b = 0;
  self->inverse = 0;
  self->last_event_time_ms = 0;
  return self;
}

void encoder_free(VirtEncoder *self) { free(self); }

void encoder_set_callback(VirtEncoder *self, EncoderCallbackFunction f) { self->event_callback = f; }

const int64_t _state_table[4][4] = {
    {0, -1, 1, 0},
    {1, 0, 0, -1},
    {-1, 0, 0, 1},
    {0, 1, -1, 0},
};

void encoder_rorate_on_time(VirtEncoder *self, bool state_a, bool state_b, uint32_t time_ms) {
  size_t last_state = (self->last_state_a << 1) | self->last_state_b;
  size_t new_state = (state_a << 1) | state_b;
  self->last_state_a = state_a;
  self->last_state_b = state_b;
  int64_t delta = _state_table[last_state][new_state];
  if (delta == 0) {
    return;
  }

  if (self->inverse) {
    delta = -delta;
  }
  self->curruent_position += delta;
  self->last_event_time_ms = time_ms;
  if (self->event_callback != NULL) {
    self->event_callback(self->id, self->curruent_position);
  }
}
