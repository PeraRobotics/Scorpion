#include "hardware.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <math.h>
#include <algorithm>
static const char *H_TAG = "HARDWARE";

// Internal MCPWM Handles
static mcpwm_timer_handle_t motor_timer = NULL;
static mcpwm_oper_handle_t m1_oper = NULL, m2_oper = NULL;
static mcpwm_cmpr_handle_t m1_cmprA = NULL, m1_cmprB = NULL;
static mcpwm_cmpr_handle_t m2_cmprA = NULL, m2_cmprB = NULL;
static mcpwm_gen_handle_t m1_genA = NULL, m1_genB = NULL;
static mcpwm_gen_handle_t m2_genA = NULL, m2_genB = NULL;

void flash_leds() {
    gpio_set_level(LED_1, 1); gpio_set_level(LED_2, 1); 
    gpio_set_level(LED_3, 1); gpio_set_level(LED_4, 1);
    vTaskDelay(pdMS_TO_TICKS(500));
    gpio_set_level(LED_1, 0); gpio_set_level(LED_2, 0); 
    gpio_set_level(LED_3, 0); gpio_set_level(LED_4, 0);
}

void set_led(gpio_num_t led_pin, bool state) {
    gpio_set_level(led_pin, state ? 1 : 0);
}

int get_button_state(gpio_num_t btn_pin) {
    return gpio_get_level(btn_pin);
}

void set_motor_speeds(float m1_speed, float m2_speed) {
    const float P = (float)MOTOR_TIMER_PERIOD_TICKS;

    // Pre-calculate ticks (2 multiplications total)
    float m1_t = fabsf(m1_speed) * P;
    float m2_t = fabsf(m2_speed) * P;

    // Branchless-style selection
    mcpwm_comparator_set_compare_value(m1_cmprA, (m1_speed >= 0.0f) ? (uint32_t)m1_t : 0);
    mcpwm_comparator_set_compare_value(m1_cmprB, (m1_speed <  0.0f) ? (uint32_t)m1_t : 0);
    mcpwm_comparator_set_compare_value(m2_cmprA, (m2_speed >= 0.0f) ? (uint32_t)m2_t : 0);
    mcpwm_comparator_set_compare_value(m2_cmprB, (m2_speed <  0.0f) ? (uint32_t)m2_t : 0);
}

void init_hardware() {
    // GPIO Init
    gpio_config(&out_conf);
    gpio_config(&in_conf);

    // MCPWM Init
    mcpwm_new_timer(&timer_config, &motor_timer);

    // Motor 1 Setup
    mcpwm_new_operator(&motor_operator_config, &m1_oper);
    mcpwm_operator_connect_timer(m1_oper, motor_timer);
    mcpwm_new_comparator(m1_oper, &cmpr_config, &m1_cmprA);
    mcpwm_new_comparator(m1_oper, &cmpr_config, &m1_cmprB);
    mcpwm_new_generator(m1_oper, &motor_1_gen_A_config, &m1_genA);
    mcpwm_new_generator(m1_oper, &motor_1_gen_B_config, &m1_genB);

    // Motor 2 Setup
    mcpwm_new_operator(&motor_operator_config, &m2_oper);
    mcpwm_operator_connect_timer(m2_oper, motor_timer);
    mcpwm_new_comparator(m2_oper, &cmpr_config, &m2_cmprA);
    mcpwm_new_comparator(m2_oper, &cmpr_config, &m2_cmprB);
    mcpwm_new_generator(m2_oper, &motor_2_gen_A_config, &m2_genA);
    mcpwm_new_generator(m2_oper, &motor_2_gen_B_config, &m2_genB);

    // Common Generator Actions
    mcpwm_gen_handle_t gens[] = {m1_genA, m1_genB, m2_genA, m2_genB};
    mcpwm_cmpr_handle_t cmprs[] = {m1_cmprA, m1_cmprB, m2_cmprA, m2_cmprB};

    for(int i=0; i<4; i++) {
        mcpwm_generator_set_action_on_timer_event(gens[i], 
            MCPWM_GEN_TIMER_EVENT_ACTION(MCPWM_TIMER_DIRECTION_UP, MCPWM_TIMER_EVENT_EMPTY, MCPWM_GEN_ACTION_HIGH));
        mcpwm_generator_set_action_on_compare_event(gens[i], 
            MCPWM_GEN_COMPARE_EVENT_ACTION(MCPWM_TIMER_DIRECTION_UP, cmprs[i], MCPWM_GEN_ACTION_LOW));
    }

    mcpwm_timer_enable(motor_timer);
    mcpwm_timer_start_stop(motor_timer, MCPWM_TIMER_START_NO_STOP);
    
    set_motor_speeds(0, 0);
    flash_leds();
    ESP_LOGI(H_TAG, "Hardware Initialized.");
}