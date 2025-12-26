#ifndef CONFIG_H
#define CONFIG_H

#include "driver/gpio.h"
#include "driver/mcpwm_prelude.h"
#include "driver/pulse_cnt.h"

// LED Pin Definitions
#define LED_1 GPIO_NUM_2 
#define LED_2 GPIO_NUM_18 
#define LED_3 GPIO_NUM_23
#define LED_4 GPIO_NUM_19

// Button Pin Definitions
#define BTN_1 GPIO_NUM_32 
#define BTN_2 GPIO_NUM_33 

// Motor Pin Definitions

// MCPWM Definition
//  └── Timer 0
//       ├── Operator 0 (Motor 1)
//       │     ├── Generator A → IN1
//       │     ├── Generator B → IN2
//       │     └── Comparator(s)
//       └── Operator 1 (Motor 2)
//             ├── Generator A → IN3
//             ├── Generator B → IN4
//             └── Comparator(s)

#define MOTOR_1_A GPIO_NUM_25 
#define MOTOR_1_B GPIO_NUM_26
#define MOTOR_2_A GPIO_NUM_27
#define MOTOR_2_B GPIO_NUM_14 

#define MOTOR_TIMER_GROUP_ID 0
#define MOTOR_TIMER_RESOLUTION_HZ 1000000
#define MOTOR_TIMER_PERIOD_TICKS 20000
#define MOTOR_OPERATOR_ID 0

// Add to your circuit:
//   10kΩ resistor from pin to 3.3V
//   0.1µF capacitor from pin to GND (for debouncing)
// #define ENC1_A GPIO_NUM_16
// #define ENC1_B GPIO_NUM_17
#define ENC1_A GPIO_NUM_34
#define ENC1_B GPIO_NUM_35
#define ENC2_A GPIO_NUM_36  
#define ENC2_B GPIO_NUM_39

#define PCNT_UNIT      PCNT_UNIT_0
#define WHEEL_CIRCUMFERENCE 0.132  // meters (example: 42mm diameter)
#define PULSES_PER_REV  360
#define PCNT_HIGH_LIMIT 32767
#define PCNT_LOW_LIMIT  -32768
#define MAX_GLITCH_NS 1000
#define DECODING_FACTOR 4.0

// Extern declarations for structures defined in config.c
// GPIO MASKS
#define INPUT_MASK  ((1ULL << BTN_1) | (1ULL << BTN_2))
#define OUTPUT_MASK ((1ULL << LED_1) | (1ULL << LED_2) | (1ULL << LED_3) | (1ULL << LED_4))
#define ENC_MASK ((1ULL << GPIO_NUM_34) | (1ULL << GPIO_NUM_35) | (1ULL << GPIO_NUM_36) | (1ULL << GPIO_NUM_39))


extern const gpio_config_t out_conf;
extern const gpio_config_t in_conf;
extern const gpio_config_t enc_conf;


extern const mcpwm_timer_config_t timer_config;
extern const mcpwm_operator_config_t motor_operator_config;
extern const mcpwm_comparator_config_t cmpr_config;
extern const mcpwm_generator_config_t motor_1_gen_A_config;
extern const mcpwm_generator_config_t motor_1_gen_B_config;
extern const mcpwm_generator_config_t motor_2_gen_A_config;
extern const mcpwm_generator_config_t motor_2_gen_B_config;

extern const pcnt_unit_config_t pcnt_unit_config;
extern const pcnt_glitch_filter_config_t filter_config;
extern const  pcnt_chan_config_t m1_chan_A_config;
extern const  pcnt_chan_config_t m1_chan_B_config;
extern const  pcnt_chan_config_t m2_chan_A_config;
extern const  pcnt_chan_config_t m2_chan_B_config;



#endif