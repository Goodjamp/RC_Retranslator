cmake_minimum_required(VERSION 3.7.0)

target_sources(${CMAKE_PROJECT_NAME}.elf PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/STM32G4xx_HAL_Driver/Src/stm32g4xx_ll_dma.c
    ${CMAKE_CURRENT_SOURCE_DIR}/STM32G4xx_HAL_Driver/Src/stm32g4xx_ll_gpio.c
    ${CMAKE_CURRENT_SOURCE_DIR}/STM32G4xx_HAL_Driver/Src/stm32g4xx_ll_pwr.c
    ${CMAKE_CURRENT_SOURCE_DIR}/STM32G4xx_HAL_Driver/Src/stm32g4xx_ll_rcc.c
    ${CMAKE_CURRENT_SOURCE_DIR}/STM32G4xx_HAL_Driver/Src/stm32g4xx_ll_spi.c
    ${CMAKE_CURRENT_SOURCE_DIR}/STM32G4xx_HAL_Driver/Src/stm32g4xx_ll_utils.c

    ${CMAKE_CURRENT_SOURCE_DIR}/startup_stm32g431xx.s
    ${CMAKE_CURRENT_SOURCE_DIR}/CMSIS/Device/ST/STM32G4xx/Source/Templates/system_stm32g4xx.c
)

target_include_directories(${CMAKE_PROJECT_NAME}.elf PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/HAL
    ${CMAKE_CURRENT_SOURCE_DIR}/STM32G4xx_HAL_Driver/src
    ${CMAKE_CURRENT_SOURCE_DIR}/STM32G4xx_HAL_Driver/inc

    ${CMAKE_CURRENT_SOURCE_DIR}/CMSIS/Include
    ${CMAKE_CURRENT_SOURCE_DIR}/CMSIS/Device/ST/STM32G4xx/Include
)
