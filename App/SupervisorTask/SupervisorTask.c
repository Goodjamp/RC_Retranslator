#include <stdint.h>

#include "FreeRTOS.h"
#include "task.h"

#include "ScreenMgr.h"

#define TASK_NAME_SUPERVISOR       "SUPERVISOR TASK"
#define TASK_DEPTH_SUPERVISOR      256
#define TASK_PRIORITY_SUPERVISOR   2

static TaskHandle_t supervisorTaskH;

static void supervisorTask(void *taskH);

GuiColor888 colorList[25*4];

static void supervisorTask(void *taskH)
{
    for (uint32_t k = 0; k < 25 * 4; k++) {
        if (k < 25 * 2) {
            colorList[k].red  = k * 5;
            colorList[k].green  = 0;
            colorList[k].blue  = k * 5;
        } else {
            colorList[k].red  = 255 - (k - 50) * 5;
            colorList[k].green  = 0;
            colorList[k].blue  = 255 - (k - 50) * 5;
        }
    }

    uint32_t k = 0;

    for(;;){

        screenMgrUpdate(colorList[k++]);
        if (k >= (sizeof(colorList) / sizeof(colorList[0]))) {
            k = 0;
        }
        vTaskDelay(40);
    }
}

void supervisorTaskInit(void)
{
    screenMgrInit();

    xTaskCreate(supervisorTask,
                TASK_NAME_SUPERVISOR,
                TASK_DEPTH_SUPERVISOR,
                NULL,
                TASK_PRIORITY_SUPERVISOR,
                &supervisorTaskH);
}