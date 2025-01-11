
#include <stdint.h>

#include "japersik/esp32_encoder/callback_funcs.h"
#include "japersik/esp32_encoder/pcnt_encoder.h"

#define ENCODER_GPIO_A 21
#define ENCODER_GPIO_B 22

void app_main(void) {
  PcntEncoder* encoder = pcnt_encoder_new(ENCODER_GPIO_A, ENCODER_GPIO_B);
  pcnt_encoder_set_callback(encoder, encoder_event_callback_print, "some_test");
}
