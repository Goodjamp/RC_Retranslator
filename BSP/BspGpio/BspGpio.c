#include <stdint.h>

#include "BSP.h"

#include "stm32g4xx_ll_spi.h"
#include "stm32g4xx_ll_gpio.h"
#include "stm32g4xx_ll_dma.h"
#include "stm32g4xx.h"

#include "BspGpio.h"
#include "BspServices.h"

typedef struct {
    GPIO_TypeDef *port;
    uint32_t pin;
} GpioDef;

static const GpioDef gpioOutList[] = {
    [BSP_GPIO_OUT_DISP_ILI9341_CS] = {CONCAT_2_WORDS(GPIO, GPIO_DISP_ILI9341_SPI_CS_PORT),
                                      CONCAT_2_WORDS(LL_GPIO_PIN_, GPIO_DISP_ILI9341_SPI_CS_PIN)
    },
    [BSP_GPIO_OUT_DISP_ILI9341_CXD] = {CONCAT_2_WORDS(GPIO, GPIO_DISP_ILI9341_CXD_PORT),
                                       CONCAT_2_WORDS(LL_GPIO_PIN_, GPIO_DISP_ILI9341_CXD_PIN)
    },
    [BSP_GPIO_OUT_DISP_ILI9341_RESET] = {CONCAT_2_WORDS(GPIO, GPIO_DISP_ILI9341_RESET_PORT),
                                         CONCAT_2_WORDS(LL_GPIO_PIN_, GPIO_DISP_ILI9341_RESET_PIN)
    },
    [BSP_GPIO_OUT_DISP_ILI9341_LED] = {CONCAT_2_WORDS(GPIO, GPIO_DISP_ILI9341_LED_PORT),
                                       CONCAT_2_WORDS(LL_GPIO_PIN_, GPIO_DISP_ILI9341_LED_PIN)
    },
};

static const GpioDef gpioInList[] = {
};

void bspGpioInit(void)
{
    LL_GPIO_InitTypeDef gpioInitStruct = {
        .Mode = LL_GPIO_MODE_OUTPUT,
        .Speed = LL_GPIO_SPEED_FREQ_VERY_HIGH,
        .OutputType = LL_GPIO_OUTPUT_PUSHPULL,
        .Pull = LL_GPIO_PULL_NO,
    };

    for (uint32_t k = 0; k < ARRAY_SIZE(gpioOutList); k++) {
        gpioInitStruct.Pin = gpioOutList[k].pin;
        bspServicesEnablePeriphery(gpioOutList[k].port);
        LL_GPIO_Init(gpioOutList[k].port, &gpioInitStruct);
    }

    gpioInitStruct.Mode = LL_GPIO_MODE_INPUT;
    for (uint32_t k = 0; k < ARRAY_SIZE(gpioInList); k++) {
        gpioInitStruct.Pin = gpioInList[k].pin;
        bspServicesEnablePeriphery(gpioInList[k].port);
        LL_GPIO_Init(gpioInList[k].port, &gpioInitStruct);
    }
}

bool bspGpioSet(BspGpioOut gpio)
{
    bool result = gpio < ARRAY_SIZE(gpioOutList);

    if (result) {
        LL_GPIO_SetOutputPin(gpioOutList[gpio].port, gpioOutList[gpio].pin);
    }

    return result;
}

bool bspGpioReset(BspGpioOut gpio)
{
    bool result = gpio < ARRAY_SIZE(gpioOutList);

    if (result) {
        LL_GPIO_ResetOutputPin(gpioOutList[gpio].port, gpioOutList[gpio].pin);
    }

    return result;
}

bool bspGpioGet(BspGpioIn gpio, bool *isSet)
{
    bool result = gpio < ARRAY_SIZE(gpioInList);

    if (result) {
        *isSet = LL_GPIO_IsInputPinSet(gpioOutList[gpio].port, gpioOutList[gpio].pin);
    }

    return result;
}
