#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>


#include "stm32g4xx_ll_gpio.h"
#include "stm32g4xx_ll_rcc.h"
#include "stm32g4xx_ll_bus.h"

#include "BspServices.h"
#include "BspDebug.h"
#include "BSP.h"

#ifdef BSP_DEBUG_ENABLE_PIN
static void debugServicesInitPin(void);

#endif

#ifdef BSP_DEBUG_ENABLE_TIME_MES
static debugServicesGetTimeCb getTimeCb = NULL;
#endif

int32_t bspDebugInit(debugServicesGetTimeCb getTime)
{
    int32_t result = BSP_DEBUG_OK;

#ifdef BSP_DEBUG_ENABLE_TIME_MES
    getTimeCb = getTime;
#else
    (void)getTime;
#endif

#ifdef BSP_DEBUG_ENABLE_PIN
    debugServicesInitPin();
#endif

    return result;
}

#ifdef BSP_DEBUG_ENABLE_PIN

static struct {
    uint32_t pin;
    GPIO_TypeDef *port;
} debugServicesPins[] = {
#if defined(DEBUG_1_GPIO_PORT) && defined(DEBUG_1_GPIO_PIN)
    [DebugPin1] = {.port = DEBUG_1_GPIO_PORT,
                   .pin = DEBUG_1_GPIO_PIN},
#endif
#if defined(DEBUG_2_GPIO_PORT) && defined(DEBUG_2_GPIO_PIN)
    [DebugPin2] = {.port = DEBUG_2_GPIO_PORT,
                   .pin = DEBUG_2_GPIO_PIN},
#endif
#if defined(DEBUG_3_GPIO_PORT) && defined(DEBUG_3_GPIO_PIN)
    [DebugPin3] = {.port = DEBUG_3_GPIO_PORT,
                   .pin = DEBUG_3_GPIO_PIN},
#endif
#if defined(DEBUG_4_GPIO_PORT) && defined(DEBUG_4_GPIO_PIN)
    [DebugPin4] = {.port = DEBUG_4_GPIO_PORT,
                   .pin = DEBUG_4_GPIO_PIN},
#endif
#if defined(DEBUG_5_GPIO_PORT) && defined(DEBUG_5_GPIO_PIN)
    [DebugPin5] = {.port = DEBUG_5_GPIO_PORT,
                   .pin = DEBUG_5_GPIO_PIN},
#endif
#if defined(DEBUG_6_GPIO_PORT) && defined(DEBUG_6_GPIO_PIN)
    [DebugPin6] = {.port = DEBUG_6_GPIO_PORT,
                   .pin = DEBUG_6_GPIO_PIN},
#endif
#if defined(DEBUG_7_GPIO_PORT) && defined(DEBUG_7_GPIO_PIN)
    [DebugPin7] = {.port = DEBUG_7_GPIO_PORT,
                   .pin = DEBUG_7_GPIO_PIN},
#endif
#if defined(DEBUG_8_GPIO_PORT) && defined(DEBUG_8_GPIO_PIN)
    [DebugPin7] = {.port = DEBUG_7_GPIO_PORT,
                   .pin = DEBUG_7_GPIO_PIN},
#endif
#if defined(DEBUG_9_GPIO_PORT) && defined(DEBUG_9_GPIO_PIN)
    [DebugPin7] = {.port = DEBUG_7_GPIO_PORT,
                   .pin = DEBUG_7_GPIO_PIN},
#endif
#if defined(DEBUG_10_GPIO_PORT) && defined(DEBUG_10_GPIO_PIN)
    [DebugPin7] = {.port = DEBUG_7_GPIO_PORT,
                   .pin = DEBUG_7_GPIO_PIN},
#endif
};

static void debugServicesInitPin(void)
{
    LL_GPIO_InitTypeDef gpioInit = {
        .Mode = LL_GPIO_MODE_OUTPUT,
        .OutputType = LL_GPIO_OUTPUT_PUSHPULL,
        .Pull = LL_GPIO_PULL_NO,
        .Speed = LL_GPIO_SPEED_FREQ_LOW,
    };

    for (uint32_t k = 0;
         k < sizeof(debugServicesPins) / sizeof(debugServicesPins[0]);
         k++) {
        gpioInit.Pin = debugServicesPins[k].pin;
        if (bspServicesEnablePeriphery(debugServicesPins[k].port) == false) {
            return;
        }
        LL_GPIO_Init(debugServicesPins[k].port, &gpioInit);
    }
}

void bspDebugPinsTest(void)
{
    bool state = true;

    for (uint32_t i = 0; i < 10; i++) {
        for (uint32_t k = 0;
             k < sizeof(debugServicesPins) / sizeof(debugServicesPins[0]);
             k++) {
            if (state) {
                bspDebugPinSet(k);
            } else {
                bspDebugPinClear(k);
            }
        }
        state = !state;
    }
}

void bspDebugPinSet(ServicesPin pin)
{
    LL_GPIO_SetOutputPin(debugServicesPins[pin].port, debugServicesPins[pin].pin);
}

void bspDebugPinClear(ServicesPin pin)
{
    LL_GPIO_ResetOutputPin(debugServicesPins[pin].port, debugServicesPins[pin].pin);
}

#else

void bspDebugPinSet(ServicesPin pin)
{
    (void)pin;
}

void bspDebugPinClear(ServicesPin pin)
{
    (void)pin;
}

#endif

#ifdef BSP_DEBUG_ENABLE_TIME_MES

void bspDebugStartMesTime(TimeMesH *timeMes)
{
    if (getTimeCb == NULL) {
        return;
    }
    *timeMes = getTimeCb();
}

uint32_t bspDebugGetTimeInterval(TimeMesH *timeMes)
{
    if (getTimeCb == NULL) {
        return 0;
    }
    return getTimeCb() - (*timeMes);
}

#else

void bspDebugStartMesTime(TimeMesH *timeMes)
{
}

uint32_t bspDebugGetTimeInterval(TimeMesH *timeMes)
{
    return 0;
}

#endif

#ifdef BSP_DEBUG_ENABLE_STATISTIC

#define STAT_MAX_INDEX    (BSP_DEBUG_STAT_SIZE_BUFF - 1)

void bspDebugFlowStatistic(bool isSuccess, FlowStatisticH *statIn)
{
    if (statIn->buff[statIn->pos]) {
        if (statIn->sucCnt) {
            statIn->sucCnt--;
        }
    } else {
        if (statIn->errCnt) {
            statIn->errCnt--;
        }
    }
    if (isSuccess) {
        statIn->sucCnt++;
    } else {
        statIn->errCnt++;
    }
    statIn->buff[statIn->pos] = isSuccess;
    statIn->pos = (statIn->pos + 1) & STAT_MAX_INDEX;
}

#else

void bspDebugFlowStatistic(bool isSuccess, FlowStatisticH *statIn)
{
    (void)isSuccess;
    (void)statIn;
}

#endif

#ifdef BSP_DEBUG_ENABLE_LOG

int32_t bspDebugLogWriteRaw(const char *message)
{
    SEGGER_RTT_WriteString(0, message);

    return 1;
}

int32_t bspDebugLogWrite(DebugServicesLogType type, const char *message)
{
    char logEntry[BSP_DEBUG_MAX_LOG_MESSAGE_SIZE + 1];

    /* We have X bytes in the log entry in total. One byte is reserved for terminating null. Size of
     * the header is Y bytes and we must subtract one from it to exclude its own null. */
    static const uint8_t maxMessagLengthOnOneLine = sizeof(logEntry) - 1
                                                    - (sizeof(BSP_DEBUG_MESS_HEADR) - 1);
    TickType_t timeExpiredSinceStartMs;
    uint8_t timeStampHours;
    uint8_t timeStampMinutes;
    uint8_t timeStampSeconds;
    uint16_t timeStampMilliseconds;
    uint8_t messageLength;

    timeExpiredSinceStartMs = xTaskGetTickCount() * pdMS_TO_TICKS(1);

    timeStampHours = (timeExpiredSinceStartMs / (60 * 60 * 1000)) % 100;
    timeExpiredSinceStartMs = timeExpiredSinceStartMs % (60 * 60 * 1000);

    timeStampMinutes = timeExpiredSinceStartMs / (60 * 1000);
    timeExpiredSinceStartMs = timeExpiredSinceStartMs % (60 * 1000);

    timeStampSeconds = timeExpiredSinceStartMs / (1000);
    timeStampMilliseconds = timeExpiredSinceStartMs % (1000);

    messageLength = strlen(message);
    if (messageLength > maxMessagLengthOnOneLine) {
        messageLength = maxMessagLengthOnOneLine;
    }

    sprintf(logEntry,
            "%02u:%02u:%02u.%02u - %c - ",
            timeStampHours, timeStampMinutes, timeStampSeconds, timeStampMilliseconds, type);
    strncat(logEntry, message, messageLength);
    strcat(logEntry, "\n\r");

    SEGGER_RTT_WriteString(0, logEntry);

    return 1;
}

#else

int32_t bspDebugLogWrite(DebugServicesLogType type, const char *message)
{
    (void)type;
    (void)message;

    return BSP_DEBUG_OK;
}

#endif
