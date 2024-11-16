#ifndef _JAPERSIK_ESP32_ENCODER_GPIO_OBSERVER_H
#define _JAPERSIK_ESP32_ENCODER_GPIO_OBSERVER_H

#define MAX_ENCODERS GPIO_NUM_MAX
struct encoderInfo;

typedef struct EncoderObserver {
} EncoderObserver;

typedef struct encoderInfo {
} encoderInfo;

EncoderObserver *encoder_observer_new();
void encoder_observer_free(EncoderObserver *observer);

void encoder_observer_add_encoder(EncoderObserver *observer);

#endif // _JAPERSIK_ESP32_ENCODER_GPIO_OBSERVER_H
