#ifndef __BSP_SYSTEM_CLOCK_H__
#define __BSP_SYSTEM_CLOCK_H__

/**
 * Init and configure the MCU clocking system.
 * The SysTick timer does not init and run on this function becouse this make on the
 * scope of vTaskStartScheduler()
 */
void bspSystemClockInit(void);

#endif