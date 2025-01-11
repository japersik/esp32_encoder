#include "japersik/esp32_encoder/virt_encoder.h"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

VirtEncoder *virt_encoder_new() {
  VirtEncoder *self = (VirtEncoder *)calloc(1, sizeof(VirtEncoder));
  if (self == NULL) {
    return NULL;
  }
  return self;
}

void virt_encoder_free(VirtEncoder *self) { free(self); }

void virt_encoder_set_callback(VirtEncoder *self, EncoderCallbackFunc f, void *c) {
  self->event_callback = f;
  self->event_callback_cxt = c;
}

const int64_t _state_table[4][4] = {
    {0, -1, 1, 0},
    {1, 0, 0, -1},
    {-1, 0, 0, 1},
    {0, 1, -1, 0},
};

void virt_encoder_rorate(VirtEncoder *self, bool state_a, bool state_b) {
  size_t last_state = (self->last_state_a << 1) | self->last_state_b;
  size_t new_state = (state_a << 1) | state_b;
  self->last_state_a = state_a;
  self->last_state_b = state_b;
  int32_t delta = _state_table[last_state][new_state];
  if (delta == 0) {
    return;
  }

  if (self->inverse) {
    delta = -delta;
  }
  self->curruent_position += delta;
  if (self->event_callback != NULL) {
    EncoderCallbackEvent event = {
	.delta = delta,
	.position = self->curruent_position,
    };
    self->event_callback(event, self->event_callback_cxt);
  }
}

void virt_encoder_change_a(VirtEncoder *self, bool state_a) { virt_encoder_rorate(self, state_a, self->last_state_b); }
void virt_encoder_change_b(VirtEncoder *self, bool state_b) { virt_encoder_rorate(self, self->last_state_a, state_b); }

