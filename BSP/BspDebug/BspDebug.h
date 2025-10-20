#ifndef __BSP_DEBUG_H__
#define __BSP_DEBUG_H__

#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>


//#define BSP_DEBUG_ENABLE_PIN
//#define BSP_DEBUG_ENABLE_LOG
//#define BSP_DEBUG_ENABLE_STATISTIC
//#define BSP_DEBUG_ENABLE_TIME_MES

#define BSP_DEBUG_OK                         0
#define BSP_DEBUG_LOG_INIT_ERROR             -1
#define BSP_DEBUG_LOG_TAKE_MUTEX_ERROR       -2
#define BSP_DEBUG_LOG_SEND_ERROR             -3
#define BSP_DEBUG_LOG_SEND_TIMEOUTE_ERROR    -4
#define BSP_DEBUG_LOG_INTERNAL_ERROR         -5
#define BSP_DEBUG_LOG_MESS_SIZE_ERROR        -7

#define BSP_DEBUG_MESS_HEADR                 "hh:mm:ss:msm - T - \n\r"
#define BSP_DEBUG_STAT_SIZE_BUFF             256
#define BSP_DEBUG_MAX_LOG_MESSAGE_SIZE       100
#define BSP_DEBUG_MESS_MAX_SIZE              (BSP_DEBUG_MAX_LOG_MESSAGE_SIZE \
                                                   - sizeof(BSP_DEBUG_MESS_HEADR) + 1)

#ifdef BSP_DEBUG_ENABLE_LOG
#define BSP_DEBUG_SEND_MESS(MESS_TYPE, ...)    {                            \
        char debugStr[BSP_DEBUG_MESS_MAX_SIZE];                             \
        if (snprintf(debugStr, BSP_DEBUG_MESS_MAX_SIZE, __VA_ARGS__) > 0) { \
            bspDebugLogWrite(MESS_TYPE, debugStr);                          \
        }                                                                        \
}
#else
#define BSP_DEBUG_SEND_MESS(MESS_TYPE, ...)
#endif

// Enumerations order must be in sync with "TaskApplicationLogType"
typedef enum {
    BSP_DEBUG_LOG_TYPE_ERROR = 'E',
    BSP_DEBUG_LOG_TYPE_INFO = 'I',
    BSP_DEBUG_LOG_TYPE_WARNING = 'W'
} DebugServicesLogType;

typedef enum {
    DebugPin1,
    DebugPin2,
    DebugPin3,
    DebugPin4,
    DebugPin5,
    DebugPin6,
    DebugPin7,
    DebugPin8,
    DebugPin9,
    DebugPin10,
} ServicesPin;

typedef struct {
    bool buff[BSP_DEBUG_STAT_SIZE_BUFF];
    uint32_t pos;
    uint32_t errCnt;
    uint32_t sucCnt;
} FlowStatisticH;

typedef uint32_t (*debugServicesGetTimeCb)(void);
typedef uint32_t TimeMesH;

int32_t bspDebugInit(debugServicesGetTimeCb getTime);
void bspDebugPinsTest(void);
void bspDebugPinSet(ServicesPin pin);
void bspDebugPinClear(ServicesPin pin);
void bspDebugStartMesTime(TimeMesH *timeMes);
uint32_t bspDebugGetTimeInterval(TimeMesH *timeMes);
void bspDebugFlowStatistic(bool isSuccess, FlowStatisticH *statIn);
int32_t bspDebugLogWrite(DebugServicesLogType type, const char *message);
int32_t bspDebugLogWriteRaw(const char *message);

#endif
