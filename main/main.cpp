#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "esp_log.h"

#include "utils/hardware.h"

#include "driver/mcpwm_prelude.h"



static const char *TAG = "SCORPION";

extern "C" void app_main(void)
{
    ESP_LOGI(TAG, "Starting Scorpion.");
    init_hardware();
    
    // Configurations

    mcpwm_timer_handle_t motor_timer = NULL;
    mcpwm_oper_handle_t motor_1_operator = NULL;
    mcpwm_cmpr_handle_t motor_1_comparator_A = NULL;
    mcpwm_cmpr_handle_t motor_1_comparator_B = NULL;
    mcpwm_gen_handle_t motor_1_generator_A = NULL;
    mcpwm_gen_handle_t motor_1_generator_B = NULL;
    mcpwm_oper_handle_t motor_2_operator = NULL;
    mcpwm_cmpr_handle_t motor_2_comparator_A = NULL;
    mcpwm_cmpr_handle_t motor_2_comparator_B = NULL;
    mcpwm_gen_handle_t motor_2_generator_A = NULL;
    mcpwm_gen_handle_t motor_2_generator_B = NULL;

    // Timer configuration
    mcpwm_timer_config_t timer_config = {};
    timer_config.group_id = MOTOR_TIMER_GROUP_ID;
    timer_config.clk_src = MCPWM_TIMER_CLK_SRC_DEFAULT;
    timer_config.resolution_hz = MOTOR_TIMER_RESOLUTION_HZ;
    timer_config.period_ticks = MOTOR_TIMER_PERIOD_TICKS;
    timer_config.count_mode = MCPWM_TIMER_COUNT_MODE_UP;

    // Operator Configuration
    mcpwm_operator_config_t motor_operator_config = {};
    motor_operator_config.group_id = MOTOR_OPERATOR_ID;

    // Comparators Configuration
    mcpwm_comparator_config_t cmpr_config = {};
    cmpr_config.flags.update_cmp_on_tez = true;

    // Generators Configuration
    mcpwm_generator_config_t motor_1_gen_A_config = {};
    motor_1_gen_A_config.gen_gpio_num = MOTOR_1_A;
    mcpwm_generator_config_t motor_1_gen_B_config = {};
    motor_1_gen_B_config.gen_gpio_num = MOTOR_1_B;
    mcpwm_generator_config_t motor_2_gen_A_config = {};
    motor_2_gen_A_config.gen_gpio_num = MOTOR_2_A;
    mcpwm_generator_config_t motor_2_gen_B_config = {};
    motor_2_gen_B_config.gen_gpio_num = MOTOR_2_B;

    // setup Timer
    mcpwm_new_timer(&timer_config, &motor_timer);

    // MOTOR 1 Setup
    mcpwm_new_operator(&motor_operator_config, &motor_1_operator);
    mcpwm_operator_connect_timer(motor_1_operator, motor_timer);
    mcpwm_new_comparator(motor_1_operator, &cmpr_config, &motor_1_comparator_A);
    mcpwm_new_comparator(motor_1_operator, &cmpr_config, &motor_1_comparator_B);
    mcpwm_new_generator(motor_1_operator, &motor_1_gen_A_config, &motor_1_generator_A);
    mcpwm_new_generator(motor_1_operator, &motor_1_gen_B_config, &motor_1_generator_B);

    // Set Generator Actions - On Start of Timer, set HIGH  | On Compare, set LOW
    mcpwm_generator_set_action_on_timer_event(motor_1_generator_A, 
        MCPWM_GEN_TIMER_EVENT_ACTION(MCPWM_TIMER_DIRECTION_UP, MCPWM_TIMER_EVENT_EMPTY, MCPWM_GEN_ACTION_HIGH));
    mcpwm_generator_set_action_on_compare_event(motor_1_generator_A, 
        MCPWM_GEN_COMPARE_EVENT_ACTION(MCPWM_TIMER_DIRECTION_UP, motor_1_comparator_A, MCPWM_GEN_ACTION_LOW));

    mcpwm_generator_set_action_on_timer_event(motor_1_generator_B , 
        MCPWM_GEN_TIMER_EVENT_ACTION(MCPWM_TIMER_DIRECTION_UP, MCPWM_TIMER_EVENT_EMPTY, MCPWM_GEN_ACTION_HIGH));
    mcpwm_generator_set_action_on_compare_event(motor_1_generator_B, 
        MCPWM_GEN_COMPARE_EVENT_ACTION(MCPWM_TIMER_DIRECTION_UP, motor_1_comparator_B, MCPWM_GEN_ACTION_LOW));

    
    // MOTOR 2 Setup
    mcpwm_new_operator(&motor_operator_config, &motor_2_operator);
    mcpwm_operator_connect_timer(motor_2_operator, motor_timer);
    mcpwm_new_comparator(motor_2_operator, &cmpr_config, &motor_2_comparator_A);
    mcpwm_new_comparator(motor_2_operator, &cmpr_config, &motor_2_comparator_B);
    mcpwm_new_generator(motor_2_operator, &motor_2_gen_A_config, &motor_2_generator_A);
    mcpwm_new_generator(motor_2_operator, &motor_2_gen_B_config, &motor_2_generator_B);

    // Set Generator Actions - On Start of Timer, set HIGH  | On Compare, set LOW
    mcpwm_generator_set_action_on_timer_event(motor_2_generator_A, 
        MCPWM_GEN_TIMER_EVENT_ACTION(MCPWM_TIMER_DIRECTION_UP, MCPWM_TIMER_EVENT_EMPTY, MCPWM_GEN_ACTION_HIGH));
    mcpwm_generator_set_action_on_compare_event(motor_2_generator_A, 
        MCPWM_GEN_COMPARE_EVENT_ACTION(MCPWM_TIMER_DIRECTION_UP, motor_2_comparator_A, MCPWM_GEN_ACTION_LOW));

    mcpwm_generator_set_action_on_timer_event(motor_2_generator_B , 
        MCPWM_GEN_TIMER_EVENT_ACTION(MCPWM_TIMER_DIRECTION_UP, MCPWM_TIMER_EVENT_EMPTY, MCPWM_GEN_ACTION_HIGH));
    mcpwm_generator_set_action_on_compare_event(motor_2_generator_B, 
        MCPWM_GEN_COMPARE_EVENT_ACTION(MCPWM_TIMER_DIRECTION_UP, motor_2_comparator_B, MCPWM_GEN_ACTION_LOW));

    // Enable and Start
    mcpwm_timer_enable(motor_timer);
    mcpwm_timer_start_stop(motor_timer, MCPWM_TIMER_START_NO_STOP);

    // Stop motor initially
    mcpwm_comparator_set_compare_value(motor_1_comparator_A, 0);
    mcpwm_comparator_set_compare_value(motor_1_comparator_B, 0);
    mcpwm_comparator_set_compare_value(motor_2_comparator_A, 0);
    mcpwm_comparator_set_compare_value(motor_2_comparator_B, 0);

    while (1) {
        // Forward (50% speed)
        mcpwm_comparator_set_compare_value(motor_1_comparator_A, 10000);
        mcpwm_comparator_set_compare_value(motor_1_comparator_B, 0); 
        mcpwm_comparator_set_compare_value(motor_2_comparator_A, 10000);
        mcpwm_comparator_set_compare_value(motor_2_comparator_B, 0);
        vTaskDelay(pdMS_TO_TICKS(2000));

        // Backward (50% speed)
        mcpwm_comparator_set_compare_value(motor_1_comparator_A, 0);
        mcpwm_comparator_set_compare_value(motor_1_comparator_B, 10000);
        mcpwm_comparator_set_compare_value(motor_2_comparator_A, 0);
        mcpwm_comparator_set_compare_value(motor_2_comparator_B, 10000);
        vTaskDelay(pdMS_TO_TICKS(2000));
    }

    // while (1) {
    //     int b1 = gpio_get_level(BTN_1);
    //     int b2 = gpio_get_level(BTN_2);

    //     %d is the placeholder for an integer
    //     ESP_LOGI(TAG, "Button States -> BTN_1: %d, BTN_2: %d", b1, b2);

    //     gpio_set_level(LED_2, b1);
    //     gpio_set_level(LED_4, b2);

    //     vTaskDelay(pdMS_TO_TICKS(50));
    // }
}