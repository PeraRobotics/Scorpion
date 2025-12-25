idf_component_register(SRCS "main.cpp"
                    INCLUDE_DIRS "."
                    REQUIRES i2c_scanner driver main esptool_py timer logger i2c ESP32_VL53L0X
                    )