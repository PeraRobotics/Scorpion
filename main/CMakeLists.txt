idf_component_register(SRCS "main.cpp"
                            "utils/config.c"
                            "utils/hardware.cpp"
                    INCLUDE_DIRS "." "utils"
                    REQUIRES 
                            driver 
                            i2c
                            util_i2c_scanner
                            sens_vl53l0x
                    )
