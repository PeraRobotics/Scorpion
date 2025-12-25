#ifndef HARDWARE_H
#define HARDWARE_H

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "esp_log.h"

// 1. Pin Definitions
#define LED1 GPIO_NUM_2 
#define LED2 GPIO_NUM_18 
#define LED3 GPIO_NUM_23
#define LED4 GPIO_NUM_19

#define BTN1 GPIO_NUM_32 
#define BTN2 GPIO_NUM_33 

#define INPUT_MASK  ((1ULL << BTN1) | (1ULL << BTN2))
#define OUTPUT_MASK ((1ULL << LED1) | (1ULL << LED2) | (1ULL << LED3) | (1ULL << LED4))


static const char *H_TAG = "HARDWARE";

inline void flash_leds() {
    gpio_set_level(LED1, 1); gpio_set_level(LED2, 1); 
    gpio_set_level(LED3, 1); gpio_set_level(LED4, 1);
    vTaskDelay(pdMS_TO_TICKS(500));
    gpio_set_level(LED1, 0); gpio_set_level(LED2, 0); 
    gpio_set_level(LED3, 0); gpio_set_level(LED4, 0);
}

inline void init_hardware() {
    gpio_reset_pin(LED1); gpio_reset_pin(LED2);
    gpio_reset_pin(LED3); gpio_reset_pin(LED4);   
    gpio_reset_pin(BTN1); gpio_reset_pin(BTN2);

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