cmake_minimum_required(VERSION 3.7.0)

target_sources(${CMAKE_PROJECT_NAME}.elf PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/main.c
    ${CMAKE_CURRENT_SOURCE_DIR}/ScreenMgr/ScreenMgr.c
    ${CMAKE_CURRENT_SOURCE_DIR}/SupervisorTask/SupervisorTask.c
)

target_include_directories(${CMAKE_PROJECT_NAME}.elf PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/ScreenMgr
    ${CMAKE_CURRENT_SOURCE_DIR}/SupervisorTask
)