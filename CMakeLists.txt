cmake_minimum_required(VERSION 3.7)
project(BBCLib)

set(CMAKE_CXX_STANDARD 14)
# To test and compile on Mac mount the arm-willtm-linux-gnueabi.dmg
if( APPLE )
    set(CMAKE_C_COMPILER ../../gcc-arm-none-eabi-8-2018-q4-major/bin/arm-none-eabi-gcc)
    set(CMAKE_CXX_COMPILER ../../gcc-arm-none-eabi-8-2018-q4-major/bin/arm-none-eabi-gcc)
endif()

set(SOURCE_FILES src/bbclib.c src/core.c src/core.h src/gpio.c src/gpio.h src/i2c.c src/i2c.h src/i2c-dev.h src/pwm.c src/pwm.h src/spi.c src/spi.h src/uart.c src/uart.h examples/gpio/gpio_example.c examples/gpio/gpio_example.h examples/i2c/i2c_example.c examples/i2c/i2c_example.h examples/leds/onboardleds.c examples/leds/onboardleds.h examples/pwm/pwm_example.c examples/pwm/pwm_example.h examples/spi/spi_example.c examples/spi/spi_example.h examples/uart/uart_example.c examples/uart/uart_example.h examples/examples.c src/log.cpp src/log.h)

set(SOURCE_FILES ${SOURCE_FILES})

add_executable(BBCLib ${SOURCE_FILES})
target_link_libraries( BBCLib )