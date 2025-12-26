#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "hardware.h"
#include "driver/pulse_cnt.h"

static const char *TAG = "SCORPION";

extern "C" void app_main(void)
{


    ESP_LOGI(TAG, "Starting Scorpion.");
    init_hardware();

    set_motor_speeds(0.5f, 1.0f);
    set_led(LED_1, true);
    vTaskDelay(pdMS_TO_TICKS(2000));
    // set_motor_speeds(0.0f, 0.0f);


    int M1_last_count = 0;
    double total_distance = 0.0;
    TickType_t last_tick = xTaskGetTickCount();

    while (1) {
        auto [M1_count, M2_count]= get_encoder_count();
        TickType_t cur_tick = xTaskGetTickCount();
        double dt = (double)(cur_tick - last_tick) / configTICK_RATE_HZ;
        int delta_pulses = M1_count - M1_last_count;
        double revs = (double)delta_pulses / (PULSES_PER_REV * DECODING_FACTOR);
        double velocity = (revs * WHEEL_CIRCUMFERENCE) / dt;
        total_distance += (revs * WHEEL_CIRCUMFERENCE);
        ESP_LOGI(TAG, "Pulses: %d | Speed: %.2f m/s | Pos: %.2f m", M1_count, velocity, total_distance);
        M1_last_count = M1_count;
        last_tick = cur_tick;
        vTaskDelay(pdMS_TO_TICKS(100));
    }
    
    // while (1) {
       
    // }
}