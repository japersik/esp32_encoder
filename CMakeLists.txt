cmake_minimum_required(VERSION 3.16)

idf_component_register(SRCS "src/virt_encoder.c" "src/pcnt_encoder.c" "src/callback_funcs.c" 
                       	INCLUDE_DIRS "include" 
												PRIV_REQUIRES driver esp_timer
)

