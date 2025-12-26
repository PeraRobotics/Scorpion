#include "config.h"

const mcpwm_timer_config_t timer_config = {
    .group_id = MOTOR_TIMER_GROUP_ID,
    .clk_src = MCPWM_TIMER_CLK_SRC_DEFAULT,
    .resolution_hz = MOTOR_TIMER_RESOLUTION_HZ,
    .period_ticks = MOTOR_TIMER_PERIOD_TICKS,
    .count_mode = MCPWM_TIMER_COUNT_MODE_UP,
};

const mcpwm_operator_config_t motor_operator_config = {
    .group_id = MOTOR_OPERATOR_ID,
};

const mcpwm_comparator_config_t cmpr_config = {
    .flags.update_cmp_on_tez = true,
};

const mcpwm_generator_config_t motor_1_gen_A_config = { .gen_gpio_num = MOTOR_1_A };
const mcpwm_generator_config_t motor_1_gen_B_config = { .gen_gpio_num = MOTOR_1_B };
const mcpwm_generator_config_t motor_2_gen_A_config = { .gen_gpio_num = MOTOR_2_A };
const mcpwm_generator_config_t motor_2_gen_B_config = { .gen_gpio_num = MOTOR_2_B };

const gpio_config_t out_conf = {
    .pin_bit_mask = OUTPUT_MASK,
    .mode = GPIO_MODE_OUTPUT,
    .pull_up_en = GPIO_PULLUP_DISABLE,
    .pull_down_en = GPIO_PULLDOWN_DISABLE,
    .intr_type = GPIO_INTR_DISABLE,
};

const gpio_config_t in_conf = {
    .pin_bit_mask = INPUT_MASK,
    .mode = GPIO_MODE_INPUT,
    .pull_up_en = GPIO_PULLUP_DISABLE,
    .pull_down_en = GPIO_PULLDOWN_ENABLE,
    .intr_type = GPIO_INTR_DISABLE,
};