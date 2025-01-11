#include "japersik/esp32_encoder/callback_funcs.h"

void encoder_event_callback_print(EncoderCallbackEvent event, void *user_ctx) {
  char *message = (char *)user_ctx;
  printf("Encoder %s in state %" PRId32 " (delta: %" PRId32 ")\n", message, event.position, event.delta);
}
