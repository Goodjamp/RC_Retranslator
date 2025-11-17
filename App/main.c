#include "FreeRTOS.h"
#include "task.h"

#include "BspSystemClock.h"
#include "BspDebug.h"
#include "BspGpio.h"
#include "SupervisorTask.h"

enum 
{ LCD_RAM_W = 20
, LCD_RAM_H = 8
, LCD_RAM_len = LCD_RAM_W * LCD_RAM_H 
};

uint8_t    LCD_RAM[LCD_RAM_len + 1];
uint8_t    LCD_COLOR[LCD_RAM_len + 1];


void main(void)
{
    bspSystemClockInit();
    bspGpioInit();
    bspDebugInit(NULL);

    vTaskStartScheduler();

    for (;;){}
}

void vApplicationDaemonTaskStartupHook( void )
{
    supervisorTaskInit();
}

/**
  * @brief This function handles System tick timer.
  */
void SysTick_Handler(void)
{
#if (INCLUDE_xTaskGetSchedulerState == 1 )
    if (xTaskGetSchedulerState() != taskSCHEDULER_NOT_STARTED)
    {
#endif
    xPortSysTickHandler();
#if (INCLUDE_xTaskGetSchedulerState == 1 )
    }
#endif
}


void vApplicationMallocFailedHook(void)
{
    /* vApplicationMallocFailedHook() will only be called if
     * configUSE_MALLOC_FAILED_HOOK is set to 1 in FreeRTOSConfig.h.  It is a hook
     * function that will get called if a call to pvPortMalloc() fails.
     * pvPortMalloc() is called internally by the kernel whenever a task, queue,
     * timer or semaphore is created.  It is also called by various parts of the
     * demo application.  If heap_1.c or heap_2.c are used, then the size of the
     * heap available to pvPortMalloc() is defined by configTOTAL_HEAP_SIZE in
     * FreeRTOSConfig.h, and the xPortGetFreeHeapSize() API function can be used
     * to query the size of free heap space that remains (although it does not
     * provide information on how the remaining heap might be fragmented).
     */
    taskDISABLE_INTERRUPTS();

    for (;;) {
    }
}