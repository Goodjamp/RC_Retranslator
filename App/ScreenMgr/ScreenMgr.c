#include <stdint.h>
#include <stdio.h>
#include "PortDisplay.h"
#include "Gui.h"
#include "ImgSrc.h"

//#include "time_fncs.h"

#include "stm32g4xx_ll_spi.h"
#include "BspSpiDispIli9341.h"


uint32_t  start_time;
uint32_t  colorList_i;
//GuiColor888 colorList[25*4];

char str[72];

//windows console colors  RGB Values
//Color Name     |      VGA      | Console Legacy |  New Default  |
//BLACK              0,   0,   0     0,   0,   0     12,  12,  12
//DARK_BLUE          0,   0, 170     0,   0, 128      0,  55, 218
//DARK_GREEN         0, 170,   0     0, 128,   0     19, 161,  14
//DARK_CYAN          0, 170, 170     0, 128, 128     58, 150, 221
//DARK_RED         170,   0,   0   128,   0,   0    197,  15,  31
//DARK_MAGENTA     170,   0, 170   128,   0, 128    136,  23, 152
//DARK_YELLOW      170,  85,   0   128, 128,   0    193, 156,   0
//DARK_WHITE       170, 170, 170   192, 192, 192    204, 204, 204
//BRIGHT_BLACK      85,  85,  85   128, 128, 128    118, 118, 118
//BRIGHT_BLUE       85,  85, 255     0,   0, 255     59, 120, 255
//BRIGHT_GREEN      85, 255,  85     0, 255,   0     22, 198,  12
//BRIGHT_CYAN       85, 255, 255     0, 255, 255     97, 214, 214
//BRIGHT_RED       255,  85,  85   255,   0,   0    231,  72,  86
//BRIGHT_MAGENTA   255,  85, 255   255,   0, 255    180,   0, 158
//BRIGHT_YELLOW    255, 255,  85   255, 255,   0    249, 241, 165
//WHITE            255, 255, 255   255, 255, 255    242, 242, 242

GuiColor888  console_colors[16] =
{ [ 0] = {  0,   0,   0},
  [ 1] = {  0,   0, 128},
  [ 2] = {  0, 128,   0},
  [ 3] = {  0, 128, 128},
  [ 4] = {128,   0,   0},
  [ 5] = {128,   0, 128},
  [ 6] = {128, 128,   0},
  [ 7] = {192, 192, 192},
  [ 8] = { 96,  96,  96},
  [ 9] = {  0,   0, 255},
  [10] = {  0, 255,   0},
  [11] = {  0, 255, 255},
  [12] = {255,   0,   0},
  [13] = {255,   0, 255},
  [14] = {255, 255,   0},
  [15] = {255, 255, 255},
};
 

void screenMgrInit(void)
{
    portDisplayInit();

    guiDrawLine((GuiColor888){0U, 64U , 0U}, 320U, 160U, 160U, 0U, 240U);
/*
    //guiDrawLine((GuiColor888){255U, 255U , 255U}, 320U, 160U, 160U, 0U, 240U);
    guiDrawLine((GuiColor888){0U, 0U , 255U}, 320U, 160U, 160U, 0U, 10U);

    GuiImg img;
    imgSrcZsyIcon(&img);
    guiAddImage(&img, (320-img.w)/2, (240-img.h)/2);

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
*/
    colorList_i = 0;
    
    //extern uint32_t  count__systick;
    
    start_time = 1;//count__systick;
}

void screenMgrPostInit(void)
{
    guiDrawLine((GuiColor888){0U, 0U , 0U}, 320U, 160U, 160U, 0U, 240U);

    for (uint32_t k = 0; k < 9; k++)
    {
        uint32_t  y = 4 + k * (28) - 3;
        guiDrawLine((GuiColor888){32U, 32U , 32U}, 320U, 160U, 160U, y, y+1);
    }
    guiSetFont(GUI_FONT_SIZE_24_mono);
    guiSetTextColor((GuiColor888){0,255,0}, (GuiColor888){0, 0, 0});

    bspSpiDispIli9341SetSpiBaudRatePrescaler(LL_SPI_BAUDRATEPRESCALER_DIV4);
}

//void screenMgrUpdate(GuiColor888 color)
void screenMgrUpdate(int y_, int x_, int n_)
{
  //extern uint32_t  count__systick;
  //static uint32_t  t = 0;
  //static uint32_t  ccc = 0;
  //uint32_t  st = count__systick;
  //if(st - t < 40) return;
  //t = st;
/*  
  guiDrawLine(colorList[colorList_i], 320U, 160U, 160U, 200U, 240U);
  colorList_i++;
  if (colorList_i >= (sizeof(colorList) / sizeof(colorList[0])))
  {
    colorList_i = 0;
  }
*/  
  
  //extern uint32_t  count__systick;
  extern uint8_t  LCD_RAM[160];
  extern uint8_t  LCD_COLOR[160];

  if(start_time != 0)
  { //if(count__systick - start_time < 2000)
    //{ return;
    //}
    screenMgrPostInit();
    start_time = 0;
  }

  //static int  k = 0;
  //for (uint32_t k = 0; k < 4; k++)
  {
    uint32_t  y = 4 + y_ * (28);
    guiSetCursor(20 + x_*14, y);
    
    uint8_t  c = LCD_COLOR[20*y_ + x_];
    guiSetTextColor(console_colors[c >> 4], console_colors[c & 0x0f]);
    guiAddTextN((char*)&LCD_RAM[20*y_ + x_], n_);
  }
  //k++;
  //if(k >= 4) k = 0;

}
