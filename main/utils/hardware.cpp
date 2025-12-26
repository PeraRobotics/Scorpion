#include "hardware.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <math.h>
#include <stdio.h>
#include <algorithm>
static const char *H_TAG = "HARDWARE";

// Internal MCPWM Handles
static mcpwm_timer_handle_t motor_timer = NULL;
static mcpwm_oper_handle_t m1_oper = NULL, m2_oper = NULL;
static mcpwm_cmpr_handle_t m1_cmprA = NULL, m1_cmprB = NULL;
static mcpwm_cmpr_handle_t m2_cmprA = NULL, m2_cmprB = NULL;
static mcpwm_gen_handle_t m1_genA = NULL, m1_genB = NULL;
static mcpwm_gen_handle_t m2_genA = NULL, m2_genB = NULL;

static pcnt_unit_handle_t m1_pcnt_unit = NULL, m2_pcnt_unit = NULL;
static pcnt_channel_handle_t m1_chan_A = NULL, m1_chan_B = NULL;
static pcnt_channel_handle_t m2_chan_A = NULL, m2_chan_B = NULL;


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

std::pair<int, int> get_encoder_count(){
    int count1 = 0;
    int count2 = 0;
    pcnt_unit_get_count(m1_pcnt_unit, &count1);
    pcnt_unit_get_count(m2_pcnt_unit, &count2);
    return std::make_pair(count1, count2);

    // USAGE
        // int M1_last_count = 0;
        // double total_distance = 0.0;
        // TickType_t last_tick = xTaskGetTickCount();

        // while (1) {
        //     auto [M1_count, M2_count]= get_encoder_count();
        //     TickType_t cur_tick = xTaskGetTickCount();
        //     double dt = (double)(cur_tick - last_tick) / configTICK_RATE_HZ;
        //     int delta_pulses = M1_count - M1_last_count;
        //     double revs = (double)delta_pulses / (PULSES_PER_REV * DECODING_FACTOR);
        //     double velocity = (revs * WHEEL_CIRCUMFERENCE) / dt;
        //     total_distance += (revs * WHEEL_CIRCUMFERENCE);
        //     ESP_LOGI(TAG, "Pulses: %d | Speed: %.2f m/s | Pos: %.2f m", M1_count, velocity, total_distance);
        //     M1_last_count = M1_count;
        //     last_tick = cur_tick;
        // }
}
void init_hardware() {
    // GPIO Init
    gpio_config(&out_conf);
    gpio_config(&in_conf);
    gpio_config(&enc_conf);  // Initialize GPIOs for encoder inputs using

    ESP_LOGI(H_TAG, "GPIO Initialized.");

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

    ESP_LOGI(H_TAG, "MCPWM Initialized.");

    // PCNT Init
    pcnt_new_unit(&pcnt_unit_config, &m1_pcnt_unit);
    pcnt_new_unit(&pcnt_unit_config, &m2_pcnt_unit);

    pcnt_unit_set_glitch_filter(m1_pcnt_unit, &filter_config);
    pcnt_unit_set_glitch_filter(m2_pcnt_unit, &filter_config);

    pcnt_new_channel(m1_pcnt_unit, &m1_chan_A_config, &m1_chan_A);
    pcnt_new_channel(m1_pcnt_unit, &m1_chan_B_config, &m1_chan_B);
    pcnt_new_channel(m2_pcnt_unit, &m2_chan_A_config, &m2_chan_A);
    pcnt_new_channel(m2_pcnt_unit, &m2_chan_B_config, &m2_chan_B);

    pcnt_channel_set_edge_action(m1_chan_A, PCNT_CHANNEL_EDGE_ACTION_DECREASE, PCNT_CHANNEL_EDGE_ACTION_INCREASE);
    pcnt_channel_set_level_action(m1_chan_A, PCNT_CHANNEL_LEVEL_ACTION_KEEP, PCNT_CHANNEL_LEVEL_ACTION_INVERSE);
    pcnt_channel_set_edge_action(m1_chan_B, PCNT_CHANNEL_EDGE_ACTION_INCREASE, PCNT_CHANNEL_EDGE_ACTION_DECREASE);
    pcnt_channel_set_level_action(m1_chan_B, PCNT_CHANNEL_LEVEL_ACTION_KEEP, PCNT_CHANNEL_LEVEL_ACTION_INVERSE);
    pcnt_channel_set_edge_action(m2_chan_A, PCNT_CHANNEL_EDGE_ACTION_DECREASE, PCNT_CHANNEL_EDGE_ACTION_INCREASE);
    pcnt_channel_set_level_action(m2_chan_A, PCNT_CHANNEL_LEVEL_ACTION_KEEP, PCNT_CHANNEL_LEVEL_ACTION_INVERSE);
    pcnt_channel_set_edge_action(m2_chan_B, PCNT_CHANNEL_EDGE_ACTION_INCREASE, PCNT_CHANNEL_EDGE_ACTION_DECREASE);
    pcnt_channel_set_level_action(m2_chan_B, PCNT_CHANNEL_LEVEL_ACTION_KEEP, PCNT_CHANNEL_LEVEL_ACTION_INVERSE);

    pcnt_unit_enable(m1_pcnt_unit);
    pcnt_unit_enable(m2_pcnt_unit);

    pcnt_unit_clear_count(m1_pcnt_unit);
    pcnt_unit_clear_count(m2_pcnt_unit);

    pcnt_unit_start(m1_pcnt_unit);
    pcnt_unit_start(m2_pcnt_unit);

    ESP_LOGI(H_TAG, "PCNT Initialized.");


    flash_leds();
    ESP_LOGI(H_TAG, "System Online.");
}