idf_component_register(SRCS "main.cpp"
                    INCLUDE_DIRS "."
                    REQUIRES i2c_scanner driver vl53l0x
                    )