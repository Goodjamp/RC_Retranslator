#include <stdint.h>
#include <stdio.h>
#include "PortDisplay.h"
#include "Gui.h"
#include "ImgSrc.h"


char str[72];

void screenMgrInit(void)
{
    portDisplayInit();

    guiDrawLine((GuiColor888){255U, 255U , 255U}, 320U, 160U, 160U, 0U, 240U);
    guiDrawLine((GuiColor888){0U, 0U , 255U}, 320U, 160U, 160U, 0U, 180U);

    //guiDrawRectangle((GuiColor888){255, 0, 0}, 2, 5, 100, 5, 100);
    //guiDrawRectangle((GuiColor888){0, 255, 0}, 2, 15, 110, 15, 110);
    //guiDrawRectangle((GuiColor888){0, 0, 255}, 2, 25, 120, 25, 120);

    //guiFillColor((GuiColor888){255, 0, 0}, 120, 170, 120, 140);
    //guiFillColor((GuiColor888){0, 255, 0}, 130, 180, 130, 150);
    //guiFillColor((GuiColor888){0, 0, 255}, 140, 190, 140, 160);

    GuiImg img;
    imgSrcZsyIcon(&img);
    guiAddImage(&img, 200, 20);

    //guiDrawLine((GuiColor888){255U, 0U , 0U}, 10U, 10U, 100U, 50U, 50U);
    //guiDrawLine((GuiColor888){0U, 255U , 0}, 10U, 100U, 100U, 100U, 250U);

    guiSetFont(GUI_FONT_SIZE_24);
    guiSetTextColor((GuiColor888){0,255,0}, (GuiColor888){0, 0, 255U});

#define START_TABLE_POS    5
    for (uint32_t k = 0; k < 5; k++) {
        uint32_t y = START_TABLE_POS + 4 + k * (29 + 5);
        guiSetCursor(5, y);
        guiSetTextColor((GuiColor888){0,255,0}, (GuiColor888){0, 0, 255U});
        snprintf(str, sizeof(str), "Частота %u", (unsigned int)k + 1u);
        guiAddText(str);
        guiSetCursor(140, y);
        guiSetTextColor((GuiColor888){0, 255, 0}, (GuiColor888){255U, 0, 0});
        snprintf(str, sizeof(str), "%u", 5860u + (unsigned int)k * 10u);
        guiAddText(str);
    }

    for (uint32_t k = 0; k < 6; k++) {
        uint32_t y = START_TABLE_POS + k * (29 + 5);
        guiDrawLine((GuiColor888){0, 0U , 0}, 2U, 0U, 320U, y, y);
    }
}

void screenMgrUpdate(GuiColor888 color)
{
    guiDrawLine(color, 320U, 160U, 160U, 200U, 240U);
}