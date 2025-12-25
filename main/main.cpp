#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "esp_log.h"

#include "utils/hardware.h"



static const char *TAG = "SCORPION";

extern "C" void app_main(void)
{
    init_hardware();

    while (1) {
        int b1 = gpio_get_level(BTN1);
        int b2 = gpio_get_level(BTN2);

        // %d is the placeholder for an integer
        ESP_LOGI(TAG, "Button States -> BTN1: %d, BTN2: %d", b1, b2);

        gpio_set_level(LED2, b1);
        gpio_set_level(LED4, b2);

        vTaskDelay(pdMS_TO_TICKS(50));
    }
}