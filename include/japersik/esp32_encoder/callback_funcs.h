#ifndef _JAPERSIK_ESP32_ENCODER_CALLBACK_FUNCS_H
#define _JAPERSIK_ESP32_ENCODER_CALLBACK_FUNCS_H

#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>

typedef struct EncoderCallbackEvent {
  int32_t position;
  int32_t delta;
} EncoderCallbackEvent;

typedef void (*EncoderCallbackFunc)(EncoderCallbackEvent event, void *user_ctx);

void encoder_event_callback_print(EncoderCallbackEvent event, void *user_ctx);

#endif // !_JAPERSIK_ESP32_ENCODER_CALLBACK_FUNCS_H
