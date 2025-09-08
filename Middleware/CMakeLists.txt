cmake_minimum_required(VERSION 3.7.0)

target_sources(${CMAKE_PROJECT_NAME}.elf
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/Display/Display.c
	
	${CMAKE_CURRENT_SOURCE_DIR}/FreeRTOS/Source/croutine.c
    ${CMAKE_CURRENT_SOURCE_DIR}/FreeRTOS/Source/event_groups.c
    ${CMAKE_CURRENT_SOURCE_DIR}/FreeRTOS/Source/list.c
    ${CMAKE_CURRENT_SOURCE_DIR}/FreeRTOS/Source/queue.c
    ${CMAKE_CURRENT_SOURCE_DIR}/FreeRTOS/Source/stream_buffer.c
    ${CMAKE_CURRENT_SOURCE_DIR}/FreeRTOS/Source/tasks.c
    ${CMAKE_CURRENT_SOURCE_DIR}/FreeRTOS/Source/timers.c
    ${CMAKE_CURRENT_SOURCE_DIR}/FreeRTOS/Source/CMSIS_RTOS/cmsis_os.c
    ${CMAKE_CURRENT_SOURCE_DIR}/FreeRTOS/Source/portable/MemMang/heap_4.c
    ${CMAKE_CURRENT_SOURCE_DIR}/FreeRTOS/Source/portable/GCC/ARM_CM4F/port.c
    
)

target_include_directories(${CMAKE_PROJECT_NAME}.elf
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/Display
	
	${CMAKE_CURRENT_SOURCE_DIR}/FreeRTOS/Source/include
    ${CMAKE_CURRENT_SOURCE_DIR}/FreeRTOS/Source/CMSIS_RTOS
    ${CMAKE_CURRENT_SOURCE_DIR}/FreeRTOS/Source/portable/GCC/ARM_CM4F
)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/Gui)