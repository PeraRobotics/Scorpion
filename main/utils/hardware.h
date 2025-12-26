#ifndef HARDWARE_H
#define HARDWARE_H

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "esp_log.h"

#include "config.h"


static const char *H_TAG = "HARDWARE";

inline void flash_leds() {
    gpio_set_level(LED_1, 1); gpio_set_level(LED_2, 1); 
    gpio_set_level(LED_3, 1); gpio_set_level(LED_4, 1);
    vTaskDelay(pdMS_TO_TICKS(500));
    gpio_set_level(LED_1, 0); gpio_set_level(LED_2, 0); 
    gpio_set_level(LED_3, 0); gpio_set_level(LED_4, 0);
}

inline void init_hardware() {
    gpio_reset_pin(LED_1); gpio_reset_pin(LED_2);
    gpio_reset_pin(LED_3); gpio_reset_pin(LED_4);   
    gpio_reset_pin(BTN_1); gpio_reset_pin(BTN_2);

    gpio_config_t out_conf = {};
    out_conf.pin_bit_mask = OUTPUT_MASK;
    out_conf.mode = GPIO_MODE_OUTPUT;
    gpio_config(&out_conf);

    gpio_config_t in_conf = {};
    in_conf.pin_bit_mask = INPUT_MASK;
    in_conf.mode = GPIO_MODE_INPUT;
    in_conf.pull_down_en = GPIO_PULLDOWN_ENABLE;
    gpio_config(&in_conf);

    flash_leds();

    ESP_LOGI(H_TAG, "GPIO Config Complete. Systems Online.");
}



#endif