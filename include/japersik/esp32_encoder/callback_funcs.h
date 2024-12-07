#ifndef _JAPERSIK_ESP32_ENCODER_CALLBACK_FUNCS_H
#define _JAPERSIK_ESP32_ENCODER_CALLBACK_FUNCS_H

#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>

typedef struct EncoderCallbackEvent {
  int32_t position;
  int32_t delta;
} EncoderCallbackEvent;

typedef void (*EncoderCallbackFunction)(EncoderCallbackEvent event, void *user_ctx);

void encoder_callback_print_event(EncoderCallbackEvent event, void *user_ctx);

#endif	// !_JAPERSIK_ESP32_ENCODER_CALLBACK_FUNCS_H
