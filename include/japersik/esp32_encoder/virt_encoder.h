#ifndef _JAPERSIK_ESP32_ENCODER_VIRT_ENCODER_H
#define _JAPERSIK_ESP32_ENCODER_VIRT_ENCODER_H

#include <stdint.h>

typedef void (*EncoderCallbackFunction)(uint8_t id, int64_t state);

typedef struct {
  EncoderCallbackFunction event_callback;
} VirtEncoder;

VirtEncoder *encoder_new(uint8_t id);
void encoder_free(VirtEncoder *);
void encoder_set_callback(VirtEncoder *, EncoderCallbackFunction);

#endif // !_JAPERSIK_ESP32_ENCODER_VIRT_ENCODER_H
