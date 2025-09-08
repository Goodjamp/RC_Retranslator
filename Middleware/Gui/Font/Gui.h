#ifndef __GUI_H__
#define __GUI_H__

#include <stdint.h>

#include "Gui.h"

typedef enum {
    GUI_STATUS_OK,
    GUI_STATUS_ERROR_CB,
    GUI_STATUS_ERROR_MALLOC,
    GUI_STATUS_ERROR_X,
    GUI_STATUS_ERROR_Y,
    GUI_STATUS_ERROR_X_Y,
    GUI_STATUS_ERROR_W,
    GUI_STATUS_ERROR_H,
    GUI_STATUS_ERROR_COLOR,
    GUI_STATUS_ERROR_FONT_SIZE,
    GUI_STATUS_ERROR_SYMBOL_NOT_FOUND,
} GuiStatus;

typedef uint16_t Utf8Code;

typedef enum {
    GUI_FONT_SIZE_16,
    GUI_FONT_SIZE_20,
    GUI_FONT_SIZE_24,
    GUI_FONT_SIZE_CNT,
} GuiFontSize;

#endif
