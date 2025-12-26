#ifndef HARDWARE_H
#define HARDWARE_H

#include <stdio.h>
#include <utility>
#include "driver/gpio.h"
#include "driver/pulse_cnt.h"
#include "config.h"

#ifdef __cplusplus
extern "C" {
#endif

// Initialization
void init_hardware();

// LED Control
void flash_leds();
void set_led(gpio_num_t led_pin, bool state);

// Input
int get_button_state(gpio_num_t btn_pin);

// Motor Control: speed from -1.0 to 1.0
void set_motor_speeds(float m1_speed, float m2_speed);

// Encorder Count
std::pair<int, int> get_encoder_count();

#ifdef __cplusplus
}
#endif

#endif