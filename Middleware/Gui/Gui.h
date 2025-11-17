#ifndef __GUI_H__
#define __GUI_H__

#include <stdint.h>

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
    GUI_STATUS_ERROR_NULL,
} GuiStatus;

#pragma pack(push,1)
typedef struct {
    uint8_t red;
    uint8_t green;
    uint8_t blue;
} GuiColor888;
#pragma pack(pop)

typedef uint16_t GuiColor565;

typedef struct {
    void (*putPixels888)(const GuiColor888 *buff, uint32_t pixelCnt);
    void (*putPixels565)(const GuiColor565 *buff, uint32_t pixelCnt);
    void (*setRegion)(uint16_t x, uint16_t y, uint16_t w, uint16_t h);
    uint8_t *(*memAlloc)(uint32_t *size);
} GuiCb;

typedef struct {
    const GuiColor565 *buff;
    uint32_t size;
    uint16_t w;
    uint16_t h;
} GuiImg;

typedef enum {
    GUI_FONT_SIZE_16,
    GUI_FONT_SIZE_20,
    GUI_FONT_SIZE_24,
    GUI_FONT_SIZE_24_mono,
    GUI_FONT_SIZE_CNT,
} GuiFontSize;

GuiStatus guiInit(GuiCb guiCb, uint16_t screenWidth, uint16_t screenHeigh);
GuiStatus guiDrawLine(GuiColor888 color, uint16_t w, uint16_t x1, uint16_t x2, uint16_t y1, uint16_t y2);
GuiStatus guiDrawRectangle(GuiColor888 color, uint16_t w, uint16_t x1, uint16_t x2, uint16_t y1, uint16_t y2);
GuiStatus guiFillColor(GuiColor888 color, uint16_t x1, uint16_t x2, uint16_t y1, uint16_t y2);
GuiStatus guiAddImage(GuiImg *img, uint16_t x, uint16_t y);
GuiStatus guiSetCursor(uint16_t x, uint16_t y);
GuiStatus guiSetFont(GuiFontSize fontSize);
GuiStatus guiSetTextColor(GuiColor888 textColor, GuiColor888 backgroundColor);
GuiStatus guiAddTextN(const char *string, int str_len_);

GuiStatus __attribute__((always_inline)) guiAddText(const char *string);

#endif
