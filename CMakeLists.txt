cmake_minimum_required(VERSION 3.16)

idf_component_register(SRCS "src/virt_encoder.c" "src/encoder_gpio_observer.c" 
                       	INCLUDE_DIRS "include" 
												PRIV_REQUIRES driver esp_timer
)

