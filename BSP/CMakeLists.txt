cmake_minimum_required(VERSION 3.7.0)

target_sources(${CMAKE_PROJECT_NAME}.elf
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/BspServices/BspServices.c
    ${CMAKE_CURRENT_SOURCE_DIR}/BspSpiDispIli9341/BspSpiDispIli9341.c
    ${CMAKE_CURRENT_SOURCE_DIR}/BspSystemClock/BspSystemClock.c
    ${CMAKE_CURRENT_SOURCE_DIR}/BspDebug/BspDebug.c
    ${CMAKE_CURRENT_SOURCE_DIR}/BspGpio/BspGpio.c
)

target_include_directories(${CMAKE_PROJECT_NAME}.elf
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/BspServices
    ${CMAKE_CURRENT_SOURCE_DIR}/BspSpiDispIli9341
    ${CMAKE_CURRENT_SOURCE_DIR}/BspSystemClock
    ${CMAKE_CURRENT_SOURCE_DIR}/BspDebug
    ${CMAKE_CURRENT_SOURCE_DIR}/BspGpio
)
