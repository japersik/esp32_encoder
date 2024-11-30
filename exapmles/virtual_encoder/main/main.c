
#include <stdio.h>

#include "driver/gpio.h"
#include "esp_timer.h"
#include "freertos/FreeRTOS.h"
#include "japersik/esp32_encoder/virt_encoder.h"
#include "portmacro.h"

#define INPUT_PIN_1 22
#define INPUT_PIN_2 23

void callback(uint8_t id, int64_t position) { printf("%d: %lld\n", id, position); };

void app_main() {
  gpio_set_direction(INPUT_PIN_1, GPIO_MODE_INPUT);
  gpio_pulldown_dis(INPUT_PIN_1);
  gpio_pullup_en(INPUT_PIN_1);

  gpio_set_direction(INPUT_PIN_2, GPIO_MODE_INPUT);
  gpio_pulldown_dis(INPUT_PIN_2);
  gpio_pullup_en(INPUT_PIN_2);

  VirtEncoder* encoder = encoder_new(1);
  encoder_set_callback(encoder, callback);
  for (;;) {
    int level1 = gpio_get_level(INPUT_PIN_1);
    int level2 = gpio_get_level(INPUT_PIN_2);
    encoder_rorate_on_time(encoder, level1, level2, 0);
    vTaskDelay(5 / portTICK_PERIOD_MS);
  }
}

