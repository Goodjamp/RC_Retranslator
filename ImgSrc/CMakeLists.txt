cmake_minimum_required(VERSION 3.7.0)

target_sources(${CMAKE_PROJECT_NAME}.elf
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/ZsyIcon.c
)

target_include_directories(${CMAKE_PROJECT_NAME}.elf
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)
