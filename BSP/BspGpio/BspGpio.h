#ifndef __BSP_GPIO_H__
#define __BSP_GPIO_H__

#include <stdbool.h>

/*
 * Define the GPIO with GPIO function
 */

typedef enum {
    BSP_GPIO_OUT_DISP_ILI9341_CS,
    BSP_GPIO_OUT_DISP_ILI9341_CXD,
    BSP_GPIO_OUT_DISP_ILI9341_RESET,
    BSP_GPIO_OUT_DISP_ILI9341_LED,
    BSP_GPIO_OUT_CNT,
} BspGpioOut;

typedef enum {
    GPIO_IN_X,
    GPIO_IN_CNT,
} BspGpioIn;

void bspGpioInit(void);
bool bspGpioSet(BspGpioOut gpio);
bool bspGpioReset(BspGpioOut gpio);
bool bspGpioGet(BspGpioIn gpio, bool *isSet);

#endif
