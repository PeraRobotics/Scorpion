#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "hardware.h"

static const char *TAG = "SCORPION";

extern "C" void app_main(void)
{
    ESP_LOGI(TAG, "Starting Scorpion.");
    init_hardware();

    set_motor_speeds(1.0f, 1.0f);
    set_led(LED_1, true);
    vTaskDelay(pdMS_TO_TICKS(2000));
    set_motor_speeds(0.0f, 0.0f);

    // while (1) {
       
    // }
}