#ifndef CONFIG_H
#define CONFIG_H

// LED Pin Definitions
#define LED_1 GPIO_NUM_2 
#define LED_2 GPIO_NUM_18 
#define LED_3 GPIO_NUM_23
#define LED_4 GPIO_NUM_19

// Button Pin Definitions
#define BTN_1 GPIO_NUM_32 
#define BTN_2 GPIO_NUM_33 

// GPIO MASKS
#define INPUT_MASK  ((1ULL << BTN_1) | (1ULL << BTN_2))
#define OUTPUT_MASK ((1ULL << LED_1) | (1ULL << LED_2) | (1ULL << LED_3) | (1ULL << LED_4))

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


#endif