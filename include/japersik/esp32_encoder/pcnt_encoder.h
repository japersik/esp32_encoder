#ifndef _JAPERSIK_ESP32_ENCODER_PCNT_ENCODER_H
#define _JAPERSIK_ESP32_ENCODER_PCNT_ENCODER_H

#include <stdbool.h>
#include <stdint.h>

#include "callback_funcs.h"
#include "hal/gpio_types.h"

typedef struct PcntEncoder PcntEncoder;

PcntEncoder *pcnt_encoder_new(gpio_port_t a, gpio_port_t b);

void pcnt_encoder_free(PcntEncoder *);
void pcnt_encoder_set_callback(PcntEncoder *, EncoderCallbackFunc, void *user_ctx);
#endif	// !_JAPERSIK_ESP32_ENCODER_PCNT_ENCODER_H
