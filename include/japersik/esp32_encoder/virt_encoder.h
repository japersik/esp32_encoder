#ifndef _JAPERSIK_ESP32_ENCODER_VIRT_ENCODER_H
#define _JAPERSIK_ESP32_ENCODER_VIRT_ENCODER_H

#include <stdbool.h>
#include <stdint.h>

#include "callback_funcs.h"

typedef struct {
  EncoderCallbackFunc event_callback;
  void *event_callback_cxt;

  int32_t curruent_position;
  bool inverse;
  bool last_state_a;
  bool last_state_b;
} VirtEncoder;

VirtEncoder *virt_encoder_new();

void virt_encoder_free(VirtEncoder *);
void virt_encoder_set_callback(VirtEncoder *, EncoderCallbackFunc,
                               void *user_ctx);
void virt_encoder_rorate(VirtEncoder *, bool a_state, bool b_state);
void virt_encoder_change_a(VirtEncoder *, bool a_state);
void virt_encoder_change_b(VirtEncoder *, bool b_state);
#endif // !_JAPERSIK_ESP32_ENCODER_VIRT_ENCODER_H
