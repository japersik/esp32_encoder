
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "japersik/esp32_encoder/callback_funcs.h"
#include "japersik/esp32_encoder/virt_encoder.h"

#define INPUT_PIN_1 21
#define INPUT_PIN_2 22

void callback(uint8_t id, int64_t position) { printf("%d: %lld\n", id, position); };

void app_main() {
  gpio_set_direction(INPUT_PIN_1, GPIO_MODE_INPUT);
  gpio_pulldown_dis(INPUT_PIN_1);
  gpio_pullup_en(INPUT_PIN_1);

  gpio_set_direction(INPUT_PIN_2, GPIO_MODE_INPUT);
  gpio_pulldown_dis(INPUT_PIN_2);
  gpio_pullup_en(INPUT_PIN_2);

  VirtEncoder* encoder = virt_encoder_new();
  virt_encoder_set_callback(encoder, encoder_event_callback_print, "test_encoder");
  for (;;) {
    int level1 = gpio_get_level(INPUT_PIN_1);
    int level2 = gpio_get_level(INPUT_PIN_2);
    virt_encoder_rorate(encoder, level1, level2);
    vTaskDelay(10 / portTICK_PERIOD_MS);
  }
}

