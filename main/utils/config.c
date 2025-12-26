#include "config.h"

// GPIO Configurations
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

// DIABLE PULLUP in encorder pins
const gpio_config_t enc_conf = {
    .pin_bit_mask = ENC_MASK,
    .mode = GPIO_MODE_INPUT,
    .pull_up_en = GPIO_PULLUP_DISABLE,
    .pull_down_en = GPIO_PULLDOWN_DISABLE,
    .intr_type = GPIO_INTR_DISABLE,
};


// MCPWM Configurations
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

// PCNT Configurations
const pcnt_unit_config_t pcnt_unit_config = {
    .low_limit = PCNT_LOW_LIMIT,
    .high_limit = PCNT_HIGH_LIMIT,
};

const pcnt_glitch_filter_config_t filter_config = { .max_glitch_ns = MAX_GLITCH_NS };

const pcnt_chan_config_t m1_chan_A_config = {
    .edge_gpio_num = ENC1_A,
    .level_gpio_num = ENC1_B,
};
const pcnt_chan_config_t m1_chan_B_config = {
    .edge_gpio_num = ENC1_B,
    .level_gpio_num = ENC1_A,
};
const pcnt_chan_config_t m2_chan_A_config = {
    .edge_gpio_num = ENC2_A,
    .level_gpio_num = ENC2_B,
};
const pcnt_chan_config_t m2_chan_B_config = {
    .edge_gpio_num = ENC2_B,
    .level_gpio_num = ENC2_A,
};

