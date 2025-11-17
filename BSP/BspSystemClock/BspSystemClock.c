#include "stm32g4xx_ll_pwr.h"
#include "stm32g4xx_ll_rcc.h"
#include "stm32g4xx_ll_gpio.h"
#include "stm32g4xx_ll_system.h"
#include "stm32g4xx_ll_utils.h"
#include "stm32g4xx_ll_bus.h"

/**
  * @brief System Clock Configuration
  * @retval None
  */
void systemClockInit_(void)
{
    SystemInit();
    LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_SYSCFG);
    LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_PWR);

    LL_FLASH_SetLatency(LL_FLASH_LATENCY_3);
    while(LL_FLASH_GetLatency()!= LL_FLASH_LATENCY_3) { }
    LL_PWR_SetRegulVoltageScaling(LL_PWR_REGU_VOLTAGE_SCALE2);

    LL_RCC_HSE_Enable();
    while(LL_RCC_HSE_IsReady() != 1) { }
    LL_RCC_PLL_ConfigDomain_SYS(LL_RCC_PLLSOURCE_HSE, LL_RCC_PLLM_DIV_8, 336, LL_RCC_PLLP_DIV_2);
    LL_RCC_PLL_Enable();

    /* Wait till PLL is ready */
    while(LL_RCC_PLL_IsReady() != 1) { }
    while (LL_PWR_IsActiveFlag_VOS() == 0) { }

    LL_RCC_SetAHBPrescaler(LL_RCC_SYSCLK_DIV_1);
    LL_RCC_SetAPB1Prescaler(LL_RCC_APB1_DIV_4);
    LL_RCC_SetAPB2Prescaler(LL_RCC_APB2_DIV_2);
    LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_PLL);

    /* Wait till System clock is ready */
    while(LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_PLL) { }
    
    SystemCoreClockUpdate();
    //LL_SetSystemCoreClock(168000);
    
    /*
     * Configure SysTick to interrupt 1ms.
     */

    LL_PWR_EnableBkUpAccess();
    LL_PWR_IsEnabledBkUpAccess();
    LL_PWR_IsEnabledBkUpAccess();
    LL_RCC_SetRTCClockSource(LL_RCC_RTC_CLKSOURCE_LSE);

/*
    LL_RCC_LSE_Enable();
    while (LL_RCC_LSE_IsReady() == 0) {}
    LL_Init1msTick(LSE_VALUE);
*/
    LL_RCC_ConfigMCO(LL_RCC_MCO1SOURCE_PLLCLK, LL_RCC_MCO1_DIV_2);
    //LL_RCC_SetTIMPrescaler(LL_RCC_TIM_PRESCALER_TWICE);
}

static void bspSystemClockInitMco(void)
{
#define GPIO_PIN_MCO1     LL_GPIO_PIN_8
#define GPIO_PORT_MCO1    GPIOA
    LL_GPIO_InitTypeDef gpioInit = {
        .Alternate = LL_GPIO_AF_0,
        .Mode = LL_GPIO_MODE_ALTERNATE,
        .OutputType = LL_GPIO_OUTPUT_PUSHPULL,
        .Pin = GPIO_PIN_MCO1,
        .Pull = LL_GPIO_PULL_NO,
        .Speed = LL_GPIO_SPEED_FREQ_VERY_HIGH,
    };

    LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_GPIOA);
    LL_GPIO_Init(GPIO_PORT_MCO1, &gpioInit);

    LL_RCC_ConfigMCO(LL_RCC_MCO1SOURCE_PLLCLK, RCC_CFGR_MCOPRE_DIV16);
}

void bspSystemClockInit(void)
{
    LL_FLASH_SetLatency(LL_FLASH_LATENCY_4);
    while(LL_FLASH_GetLatency() != LL_FLASH_LATENCY_4) {
    }
    LL_PWR_EnableRange1BoostMode();
    LL_RCC_HSE_Enable();
    /* Wait till HSE is ready */
    while(LL_RCC_HSE_IsReady() != 1) {
    }

    LL_RCC_PLL_ConfigDomain_SYS(LL_RCC_PLLSOURCE_HSE, LL_RCC_PLLM_DIV_4, 85, LL_RCC_PLLR_DIV_2);
    LL_RCC_PLL_EnableDomain_SYS();
    LL_RCC_PLL_Enable();
    /* Wait till PLL is ready */
    while(LL_RCC_PLL_IsReady() != 1) {
    }

    LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_PLL);
    LL_RCC_SetAHBPrescaler(LL_RCC_SYSCLK_DIV_2);
    /* Wait till System clock is ready */
    while(LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_PLL) {
    }

    /* Insure 1us transition state at intermediate medium speed clock*/
    for (__IO uint32_t i = (170 >> 1); i !=0; i--);

    /* Set AHB prescaler*/
    LL_RCC_SetAHBPrescaler(LL_RCC_SYSCLK_DIV_1);
    LL_RCC_SetAPB1Prescaler(LL_RCC_APB1_DIV_1);
    LL_RCC_SetAPB2Prescaler(LL_RCC_APB2_DIV_1);

    //LL_Init1msTick(170000000);

    LL_SetSystemCoreClock(170000000);

    //SysTick_Config(170000);
    bspSystemClockInitMco();
}
