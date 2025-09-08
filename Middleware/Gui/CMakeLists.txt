cmake_minimum_required(VERSION 3.7.0)

target_sources(${CMAKE_PROJECT_NAME}.elf
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/Gui.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Font/Font.c
#    ${CMAKE_CURRENT_SOURCE_DIR}/Font/Font16.c
#    ${CMAKE_CURRENT_SOURCE_DIR}/Font/Font20.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Font/Font24_Compress.c
)

target_include_directories(${CMAKE_PROJECT_NAME}.elf
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/Font
)
