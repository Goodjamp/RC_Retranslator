#include <string.h>

#include "stm32g4xx.h"
#include "stm32g4xx_ll_bus.h"
#include "stm32g4xx_ll_rcc.h"
#include "stm32g4xx_ll_utils.h"

#include "BspServices.h"

uint32_t bspServicesGetTimOscFrq(void *baseTim)
{
    uint32_t timAddress;
    uint32_t resFrq = 0;
    LL_RCC_ClocksTypeDef sysClocks;

    memcpy(&timAddress, &baseTim, sizeof(timAddress));
    LL_RCC_GetSystemClocksFreq(&sysClocks);
    resFrq = sysClocks.HCLK_Frequency;

    switch (timAddress) {
    case TIM1_BASE:
    case TIM8_BASE:
        if(LL_RCC_GetAPB2Prescaler()) {
            resFrq = sysClocks.PCLK2_Frequency * 2;
        }
        break;

    case TIM2_BASE:
    case TIM3_BASE:
    case TIM4_BASE:
    case TIM6_BASE:
    case TIM7_BASE:
        if(LL_RCC_GetAPB1Prescaler()) {
            resFrq = sysClocks.PCLK1_Frequency * 2;
        }
        break;
    }

    return resFrq;
}

bool bspServicesEnablePeriphery(void *pereph)
{
    uint32_t perepAddress;
    bool result = true;

	memcpy(&perepAddress, &pereph, sizeof(perepAddress));

    switch (perepAddress) {
    case TIM2_BASE:
        LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_TIM2);
        break;

    case TIM3_BASE:
        LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_TIM3);
        break;

    case TIM4_BASE:
        LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_TIM4);
        break;

    case TIM6_BASE:
        LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_TIM6);
        break;

    case TIM7_BASE:
        LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_TIM7);
        break;

    case WWDG_BASE:
        LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_WWDG);
        break;

    case SPI2_BASE:
        LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_SPI2);
        break;

    case SPI3_BASE:
        LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_SPI3);
        break;

    case USART2_BASE:
        LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_USART2);
        break;

    case USART3_BASE:
        LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_USART3);
        break;

    case UART4_BASE:
        LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_UART4);
        break;

    case I2C1_BASE:
        LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_I2C1);
        break;

    case I2C2_BASE:
        LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_I2C2);
        break;

    case I2C3_BASE:
        LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_I2C3);
        break;

    case PWR_BASE:
        LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_PWR);
        break;

    case LPUART1_BASE:
        LL_APB1_GRP2_EnableClock(LL_APB1_GRP2_PERIPH_LPUART1);
        break;

    case TIM1_BASE:
        LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_TIM1);
        break;

    case USART1_BASE:
        LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_USART1);
        break;

    case ADC1_BASE:
        LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_ADC12);
        break;

    case SPI1_BASE:
        LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_SPI1);
        break;

    case SYSCFG_BASE:
        LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_SYSCFG);
        break;

    case GPIOA_BASE:
        LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_GPIOA);
        break;

    case GPIOB_BASE:
        LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_GPIOB);
        break;

    case GPIOC_BASE:
        LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_GPIOC);
        break;

    case GPIOD_BASE:
        LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_GPIOD);
        break;

    case GPIOE_BASE:
        LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_GPIOE);
        break;

    case GPIOF_BASE:
        LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_GPIOF);
        break;

    case CRC_BASE:
        LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_CRC);
        break;

    case DMA1_BASE:
        LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_DMA1);
        break;

    case DMA2_BASE:
        LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_DMA2);
        break;

    case DMAMUX1_BASE:
        LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_DMAMUX1);
        break;

    default:
        result = false;
        break;
    }

    return result;
}

bool bspServicesDisablePerephr(void *pereph)
{
    uint32_t perepAddress;
    memcpy(&perepAddress, &pereph, sizeof(perepAddress));
    bool result = true;

    switch (perepAddress) {
    case TIM2_BASE:
        LL_APB1_GRP1_DisableClock(LL_APB1_GRP1_PERIPH_TIM2);
        break;

    case TIM3_BASE:
        LL_APB1_GRP1_DisableClock(LL_APB1_GRP1_PERIPH_TIM3);
        break;

    case TIM4_BASE:
        LL_APB1_GRP1_DisableClock(LL_APB1_GRP1_PERIPH_TIM4);
        break;

    case TIM6_BASE:
        LL_APB1_GRP1_DisableClock(LL_APB1_GRP1_PERIPH_TIM6);
        break;

    case TIM7_BASE:
        LL_APB1_GRP1_DisableClock(LL_APB1_GRP1_PERIPH_TIM7);
        break;

    case WWDG_BASE:
        LL_APB1_GRP1_DisableClock(LL_APB1_GRP1_PERIPH_WWDG);
        break;

    case SPI2_BASE:
        LL_APB1_GRP1_DisableClock(LL_APB1_GRP1_PERIPH_SPI2);
        break;

    case SPI3_BASE:
        LL_APB1_GRP1_DisableClock(LL_APB1_GRP1_PERIPH_SPI3);
        break;

    case USART2_BASE:
        LL_APB1_GRP1_DisableClock(LL_APB1_GRP1_PERIPH_USART2);
        break;

    case USART3_BASE:
        LL_APB1_GRP1_DisableClock(LL_APB1_GRP1_PERIPH_USART3);
        break;

    case UART4_BASE:
        LL_APB1_GRP1_DisableClock(LL_APB1_GRP1_PERIPH_UART4);
        break;

    case I2C1_BASE:
        LL_APB1_GRP1_DisableClock(LL_APB1_GRP1_PERIPH_I2C1);
        break;

    case I2C2_BASE:
        LL_APB1_GRP1_DisableClock(LL_APB1_GRP1_PERIPH_I2C2);
        break;

    case I2C3_BASE:
        LL_APB1_GRP1_DisableClock(LL_APB1_GRP1_PERIPH_I2C3);
        break;

    case PWR_BASE:
        LL_APB1_GRP1_DisableClock(LL_APB1_GRP1_PERIPH_PWR);
        break;

    case TIM1_BASE:
        LL_APB2_GRP1_DisableClock(LL_APB2_GRP1_PERIPH_TIM1);
        break;

    case USART1_BASE:
        LL_APB2_GRP1_DisableClock(LL_APB2_GRP1_PERIPH_USART1);
        break;

    case ADC1_BASE:
        LL_AHB2_GRP1_DisableClock(LL_AHB2_GRP1_PERIPH_ADC12);
        break;

    case SPI1_BASE:
        LL_APB2_GRP1_DisableClock(LL_APB2_GRP1_PERIPH_SPI1);
        break;

    case SYSCFG_BASE:
        LL_APB2_GRP1_DisableClock(LL_APB2_GRP1_PERIPH_SYSCFG);
        break;

    case GPIOA_BASE:
        LL_AHB2_GRP1_DisableClock(LL_AHB2_GRP1_PERIPH_GPIOA);
        break;

    case GPIOB_BASE:
        LL_AHB2_GRP1_DisableClock(LL_AHB2_GRP1_PERIPH_GPIOB);
        break;

    case GPIOC_BASE:
        LL_AHB2_GRP1_DisableClock(LL_AHB2_GRP1_PERIPH_GPIOC);
        break;

    case GPIOD_BASE:
        LL_AHB2_GRP1_DisableClock(LL_AHB2_GRP1_PERIPH_GPIOD);
        break;

    case CRC_BASE:
        LL_AHB1_GRP1_DisableClock(LL_AHB1_GRP1_PERIPH_CRC);
        break;

    case DMA1_BASE:
        LL_AHB1_GRP1_DisableClock(LL_AHB1_GRP1_PERIPH_DMA1);
        break;

    case DMA2_BASE:
        LL_AHB1_GRP1_DisableClock(LL_AHB1_GRP1_PERIPH_DMA2);
        break;

    default:
        result = false;
        break;
    }

    return result;
}

void bspServicesDmaClearFlagTE(DMA_TypeDef *dma, uint32_t stream)
{
    static void (*const clearFlag[])(DMA_TypeDef *) = {
        LL_DMA_ClearFlag_TE1,
        LL_DMA_ClearFlag_TE2, LL_DMA_ClearFlag_TE3,
        LL_DMA_ClearFlag_TE4, LL_DMA_ClearFlag_TE5,
        LL_DMA_ClearFlag_TE6,
    };

    clearFlag[stream](dma);
}

void bspServicesDmaClearFlagHT(DMA_TypeDef *dma, uint32_t stream)
{
    static void (*const clearFlag[])(DMA_TypeDef *) = {
        LL_DMA_ClearFlag_HT1,
        LL_DMA_ClearFlag_HT2, LL_DMA_ClearFlag_HT3,
        LL_DMA_ClearFlag_HT4, LL_DMA_ClearFlag_HT5,
        LL_DMA_ClearFlag_HT6,
    };

    clearFlag[stream](dma);
}

void bspServicesDmaClearFlagTC(DMA_TypeDef *dma, uint32_t stream)
{
    static void (*const clearFlag[])(DMA_TypeDef *) = {
        LL_DMA_ClearFlag_TC1,
        LL_DMA_ClearFlag_TC2, LL_DMA_ClearFlag_TC3,
        LL_DMA_ClearFlag_TC4, LL_DMA_ClearFlag_TC5,
        LL_DMA_ClearFlag_TC6,
    };

    clearFlag[stream](dma);
}

void bspServicesDmaClearFlagGI(DMA_TypeDef *dma, uint32_t stream)
{
    static void (*const clearFlag[])(DMA_TypeDef *) = {
        LL_DMA_ClearFlag_GI1,
        LL_DMA_ClearFlag_GI2, LL_DMA_ClearFlag_GI3,
        LL_DMA_ClearFlag_GI4, LL_DMA_ClearFlag_GI5,
        LL_DMA_ClearFlag_GI6,
    };

    clearFlag[stream](dma);
}

uint32_t bspServicesDmaIsActiveFlagTC(DMA_TypeDef *dma, uint32_t stream)
{
    static uint32_t (*const getFlag[])(DMA_TypeDef *) = {
        LL_DMA_IsActiveFlag_TC1,
        LL_DMA_IsActiveFlag_TC2, LL_DMA_IsActiveFlag_TC3,
        LL_DMA_IsActiveFlag_TC4, LL_DMA_IsActiveFlag_TC5,
        LL_DMA_IsActiveFlag_TC6,
    };

    return getFlag[stream](dma);
}

uint32_t bspServicesDmaIsActiveFlagGI(DMA_TypeDef *dma, uint32_t stream)
{
    static uint32_t (*const getFlag[])(DMA_TypeDef *) = {
        LL_DMA_IsActiveFlag_GI1,
        LL_DMA_IsActiveFlag_GI2, LL_DMA_IsActiveFlag_GI3,
        LL_DMA_IsActiveFlag_GI4, LL_DMA_IsActiveFlag_GI5,
        LL_DMA_IsActiveFlag_GI6,
    };

    return getFlag[stream](dma);
}
