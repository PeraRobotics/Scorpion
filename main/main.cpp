// #include "i2c_scanner.hpp"
// #include "esp_log.h"
// #include "freertos/FreeRTOS.h"
// #include "freertos/task.h"


// extern "C" void app_main() { 
//     static const char* TAG = "main";

//     ESP_LOGI(TAG, "Starting application");

//     i2c_scanner::I2cScanner scanner({
//         .sda_io_num = GPIO_NUM_21,
//         .scl_io_num = GPIO_NUM_22,
//         .i2c_port = I2C_NUM_0,
//         .clk_speed = 100000
//     });

//     scanner.scan();

//     while (true) {
//         vTaskDelay(pdMS_TO_TICKS(10000));
//         scanner.scan();
//     }
// }




// // #include "vl53l0x.hpp"

// // static const char* TAG = "main";

// // extern "C" void app_main() {
// //     VL53L0X sensor({
// //         .port = I2C_NUM_0,
// //         .sda = GPIO_NUM_21,
// //         .scl = GPIO_NUM_22
// //     });

// //     if (!sensor.init()) {
// //         ESP_LOGE(TAG, "Failed to initialize VL53L0X");
// //         return;
// //     }

// //     while (true) {
// //         auto dist = sensor.read_range_single_mm();
// //         if (dist.has_value()) {
// //             ESP_LOGI(TAG, "Distance: %d mm", dist.value());
// //         } else {
// //             ESP_LOGW(TAG, "Timeout or invalid reading");
// //         }
// //         vTaskDelay(pdMS_TO_TICKS(100));
// //     }
// // }


#include "i2c_scanner.hpp"
#include "vl53l0x.hpp"          // Your custom VL53L0X component
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

static const char* TAG = "main";

// Helper: check if VL53L0X is present by trying to read its model ID
static bool vl53l0x_detected(i2c_port_t port) {
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, (0x29 << 1) | I2C_MASTER_WRITE, true);
    i2c_master_write_byte(cmd, 0xC0, true);  // MODEL_ID register
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, (0x29 << 1) | I2C_MASTER_READ, true);
    uint8_t model_id;
    i2c_master_read_byte(cmd, &model_id, I2C_MASTER_NACK);
    i2c_master_stop(cmd);
    esp_err_t ret = i2c_master_cmd_begin(port, cmd, pdMS_TO_TICKS(50));
    i2c_cmd_link_delete(cmd);

    if (ret == ESP_OK && model_id == 0xEE) {
        return true;
    }
    return false;
}

uint8_t raw_read_reg(uint8_t reg) {
    uint8_t value = 0xFF;
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, (0x29 << 1) | I2C_MASTER_WRITE, true);
    i2c_master_write_byte(cmd, reg, true);
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, (0x29 << 1) | I2C_MASTER_READ, true);
    i2c_master_read_byte(cmd, &value, I2C_MASTER_NACK);
    i2c_master_stop(cmd);
    esp_err_t ret = i2c_master_cmd_begin(I2C_NUM_0, cmd, pdMS_TO_TICKS(100));
    i2c_cmd_link_delete(cmd);
    if (ret == ESP_OK) {
        ESP_LOGI(TAG, "Raw read reg 0x%02X: 0x%02X", reg, value);
        return value;
    } else {
        ESP_LOGE(TAG, "Raw read failed for reg 0x%02X", reg);
        return 0xFF;
    }
}


extern "C" void app_main() {
    ESP_LOGI(TAG, "Starting application");

    // Initialize I2C scanner (uses port 0, pins 21/22, 100kHz)
    i2c_scanner::I2cScanner scanner({
        .sda_io_num = GPIO_NUM_21,
        .scl_io_num = GPIO_NUM_22,
        .i2c_port = I2C_NUM_0,
        .clk_speed = 100000
    });

    // First scan at startup
    scanner.scan();


    raw_read_reg(0xC0);  // Should be 0xEE for VL53L0X
    raw_read_reg(0xC1);  // Usually 0xAA
    raw_read_reg(0xC2);

    // Try to initialize VL53L0X only if it's actually present
    VL53L0X* sensor = nullptr;
    if (vl53l0x_detected(I2C_NUM_0)) {
        sensor = new VL53L0X({
            .port = I2C_NUM_0,
            .sda = GPIO_NUM_21,
            .scl = GPIO_NUM_22,
            .clock_speed = 400000  // VL53L0X supports Fast Mode (400kHz)
        });

        if (sensor->init()) {
            ESP_LOGI(TAG, "VL53L0X initialized successfully – starting distance readings");
        } else {
            ESP_LOGE(TAG, "VL53L0X found but initialization failed");
            delete sensor;
            sensor = nullptr;
        }
    } else {
        ESP_LOGW(TAG, "No VL53L0X detected at 0x29 – only running I2C scanner");
    }

    // Main loop: scan every 10 seconds + read distance if available
    while (true) {
        vTaskDelay(pdMS_TO_TICKS(10000));

        // Periodic I2C scan (your original behavior)
        scanner.scan();

        // If we have a working VL53L0X, read distance
        if (sensor) {
            auto dist = sensor->read_range_single_mm();
            if (dist.has_value()) {
                uint16_t mm = dist.value();
                ESP_LOGI(TAG, "=== Distance: %u mm (%.1f cm) ===", mm, mm / 10.0f);
            } else {
                ESP_LOGW(TAG, "VL53L0X: Measurement timeout or out of range (>8m)");
            }
        }
    }
}