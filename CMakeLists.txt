cmake_minimum_required(VERSION 3.7.0)

set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_VERSION 1)
set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)
set(CMAKE_SYSTEM_PROCESSOR arm)

set(CMAKE_C_COMPILER "arm-none-eabi-gcc")
set(CMAKE_CXX_COMPILER "arm-none-eabi-g++")
set(CMAKE_ASM_COMPILER "arm-none-eabi-gcc")

project(RC_Retranslator CXX C ASM)    # after this command the variable *CMAKE_PROJECT_NAME* will be aqual to the stmTest, and we can used it as a name of project

set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)

set(COMPILER_GENERAL_FLAGS "-mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -fno-common")
set(COMPILER_USER_FLAGS    " -O0  -g3 -Wall -Wno-multichar -fdata-sections -ffunction-sections -fno-strict-aliasing -ffreestanding -fno-builtin -fno-common")

add_executable(${CMAKE_PROJECT_NAME}.elf)# ${APP_SRC} ${BSP_SRC} ${HAL_SRC} ${MCU_SRC})

#set compile option for each copiler type: C, CPP and so on
set(CMAKE_C_FLAGS "${COMPILER_GENERAL_FLAGS} ${COMPILER_USER_FLAGS} -std=gnu99")
set(CMAKE_CXX_FLAGS "${COMPILER_GENERAL_FLAGS} ${COMPILER_USER_FLAGS}")

target_compile_definitions(${CMAKE_PROJECT_NAME}.elf PUBLIC 
	USE_FULL_LL_DRIVER 
	HSE_VALUE=8000000
	HSE_STARTUP_TIMEOUT=100 
	LSE_STARTUP_TIMEOUT=5000 
	LSE_VALUE=32768 
	EXTERNAL_CLOCK_VALUE=12288000 
	HSI_VALUE=16000000 
	LSI_VALUE=32000 
	VDD_VALUE=3300 
	PREFETCH_ENABLE=0 
	INSTRUCTION_CACHE_ENABLE=1 
	DATA_CACHE_ENABLE=1 
	STM32G431xx
    $<$<CONFIG:Debug>:DEBUG>
)

#add linker options
set (CMAKE_EXE_LINKER_FLAGS "-Xlinker -print-memory-usage -Xlinker -Map=output.map -Xlinker --gc-sections -Xlinker --sort-section=alignment -Xlinker --cref")

target_link_options(${CMAKE_PROJECT_NAME}.elf PUBLIC
    -T${CMAKE_SOURCE_DIR}/HAL/stm32g431cbux_flash.ld
    --specs=nosys.specs
    --specs=nano.specs
    -fdata-sections
    -ffunction-sections
    -lc
    -lm
    -lnosys

    -mcpu=cortex-m4
    -mfloat-abi=hard
)

add_subdirectory(${CMAKE_SOURCE_DIR}/App)
add_subdirectory(${CMAKE_SOURCE_DIR}/BSP)
add_subdirectory(${CMAKE_SOURCE_DIR}/HAL)
add_subdirectory(${CMAKE_SOURCE_DIR}/Port)
add_subdirectory(${CMAKE_SOURCE_DIR}/Middleware)
add_subdirectory(${CMAKE_SOURCE_DIR}/ImgSrc)

#print sources list 
get_target_property(SRC_LIST_ ${CMAKE_PROJECT_NAME}.elf INCLUDE_DIRECTORIES)
message(STATUS "Final sources for target: ${SRC_LIST_}")
