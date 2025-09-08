#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#include "Gui.h"
#include "Font.h"

static uint16_t scrW;
static uint16_t scrH;
static GuiCb  cb;
static uint8_t *memBuff;
static uint32_t memSize;
static uint32_t mem565PixelCnt;
static struct {
    uint16_t x;
    uint16_t y;
    uint8_t fontColorMsb;
    uint8_t fontColorLsb;
    uint8_t backColorMsb;
    uint8_t backColorLsb;
    GuiFontSize size;
} text;


#define GUI_COLOR_888_TO_565(R,G,B)    (       ((0xFF &((R & 0xF8) | (G >> 5))) << 8)  |   (0xFF & (((G & 0x1C) << 3) | (B >> 3)))        )

static GuiStatus guiFillregion(GuiColor888 color, uint16_t x, uint16_t y, uint16_t w, uint16_t h)
{
    cb.setRegion(x, y, w, h);
    uint32_t framePixelCnt = w * h; // region pixel count 

    /*
     * Fill color target region
     */
    GuiColor565 colorFill = GUI_COLOR_888_TO_565(color.red, color.green, color.blue);
    uint8_t colorMsb = 0xFF & colorFill;
    uint8_t colorLsb = 0xFF & (colorFill >> 8);

    uint16_t chunkPixelCnt = (framePixelCnt > mem565PixelCnt) ? mem565PixelCnt : framePixelCnt;
    for (uint32_t k = 0; k < chunkPixelCnt * sizeof(GuiColor565);) {
        memBuff[k++] = colorMsb;
        memBuff[k++] = colorLsb;
    }

    uint16_t tempPixelCnt = 0;
    for (; framePixelCnt; ) {
        tempPixelCnt = (framePixelCnt > chunkPixelCnt) ? chunkPixelCnt : framePixelCnt;
        cb.putPixels565((GuiColor565 *)memBuff, tempPixelCnt);
        framePixelCnt -= tempPixelCnt;
    }

    return GUI_STATUS_OK;
}

GuiStatus guiSetCursor(uint16_t x, uint16_t y)
{
    if (x > scrW) {
        return GUI_STATUS_ERROR_X;
    }
    if (y > scrH) {
        return GUI_STATUS_ERROR_Y;
    }
    text.x = x;
    text.y = y;

    return GUI_STATUS_OK;
}

GuiStatus guiSetFont(GuiFontSize fontSize)
{
    if (fontSize > GUI_FONT_SIZE_CNT) {
        return GUI_STATUS_ERROR_FONT_SIZE;
    }
    text.size = fontSize;

    return GUI_STATUS_OK;
}

GuiStatus guiSetTextColor(GuiColor888 textColor, GuiColor888 backgroundColor)
{

    GuiColor565 colorText = GUI_COLOR_888_TO_565(textColor.red, textColor.green, textColor.blue);
    text.fontColorMsb = 0xFF & colorText;
    text.fontColorLsb = 0xFF & (colorText >> 8);

    GuiColor565 colorBack = GUI_COLOR_888_TO_565(backgroundColor.red,
                                                 backgroundColor.green,
                                                 backgroundColor.blue);
    text.backColorMsb = 0xFF & colorBack;
    text.backColorLsb = 0xFF & (colorBack >> 8);

    return GUI_STATUS_OK;
}

GuiStatus guiInit(GuiCb guiCb, uint16_t screenWidth, uint16_t screenHeigh)
{
    if (guiCb.putPixels565 == NULL
        || guiCb.putPixels888 == NULL
        || guiCb.setRegion == NULL
        || guiCb.memAlloc == NULL) {
        return GUI_STATUS_ERROR_CB;
    }

    cb = guiCb;
    scrW = screenWidth;
    scrH = screenHeigh;
    memBuff = cb.memAlloc(&memSize);
    
    if (memBuff == NULL
        || memSize < sizeof(GuiColor888)) {
        return GUI_STATUS_ERROR_MALLOC; 
    }
    mem565PixelCnt = memSize / 2;

    return GUI_STATUS_OK;
}

GuiStatus guiDrawLine(GuiColor888 color, uint16_t wLine, uint16_t x1, uint16_t x2, uint16_t y1, uint16_t y2)
{
    if (x1 > scrW || x2 > scrW) {
        return GUI_STATUS_ERROR_X;
    }
    if (y1 > scrH || y2 > scrH) {
        return GUI_STATUS_ERROR_Y;
    }
    if (x1 == x2 && y1 == y2) {
        return GUI_STATUS_ERROR_X_Y;
    }
    if (wLine == 0) {
        return GUI_STATUS_ERROR_W;
    }

    if (x1 == x2 || y1 == y2) {
        uint16_t dx = (x2 > x1) ? (x2 - x1) : (x1 - x2);
        uint16_t dy = (y2 > y1) ? (y2 - y1) : (y1 - y2);
        GuiStatus result;

        if (dx) { // Draw horizontal line
            result = guiFillregion(color, x1, y1 - wLine / 2, dx, wLine);
        } else { // Draw vertical line
            result = guiFillregion(color, x1 - wLine / 2, y1, wLine, dy);
        }

        return result;
    } else {
        // TODO add line drawing if angle not aequal to 0, 90, 180, 270
    }

    return GUI_STATUS_OK;
}

GuiStatus guiDrawRectangle(GuiColor888 color, uint16_t w, uint16_t x1, uint16_t x2, uint16_t y1, uint16_t y2)
{
    GuiStatus result;
    uint16_t extendX1 = x1 - w / 2;
    uint16_t extendX2 = x2 + w / 2;
    uint16_t extendY1 = y1 - w / 2;
    uint16_t extendY2 = y2 + w / 2;

    result = guiDrawLine(color, w, extendX1, extendX2, y1, y1); // horizontal up
    if (result != GUI_STATUS_OK) {
        return result;
    }
    result = guiDrawLine(color, w, extendX1, extendX2, y2, y2); // horizontal down
    if (result != GUI_STATUS_OK) {
        return result;
    }
    result = guiDrawLine(color, w, x1, x1, extendY1, y2); // vertical left
    if (result != GUI_STATUS_OK) {
        return result;
    }
    return guiDrawLine(color, w, x2, x2, extendY1, extendY2); // vertical right
}

GuiStatus guiFillColor(GuiColor888 color1, uint16_t x1, uint16_t x2, uint16_t y1, uint16_t y2)
{
    if (x1 > x2) {
        return GUI_STATUS_ERROR_X;
    }
    if (y1 > y2) {
        return GUI_STATUS_ERROR_Y;
    }

    return guiFillregion(color1, x1, y1, x2 - x1 + 1, y2 - y1 + 1);
}

GuiStatus guiAddText(const char *string)
{
    Symbol symbol;
    uint32_t framePixelCnt = 0;
    uint16_t chunkPixelCnt = 0;
    Utf8Code utf8Code;

    while (*string) {
        utf8Code = 0;
        /*
         * Decode UTF8 symbol
         */
        if ((*string & 0xC0) == 0xC0) {
            utf8Code = (Utf8Code)((*string << 8) | *(string + 1));
            string += 2;
        } else {
            utf8Code = (Utf8Code)(*string);
            string++;
        }
        if (guiFontGetSymbol(utf8Code, text.size, &symbol) != GUI_STATUS_OK) {
            continue;
        };
        framePixelCnt = symbol.h * symbol.w;

        if ((text.x + symbol.w) >= scrW) {
            text.x = 0;
            text.y += symbol.h;
        }
        cb.setRegion(text.x, text.y, symbol.w, symbol.h);
        
        chunkPixelCnt = (framePixelCnt > mem565PixelCnt) ? mem565PixelCnt : framePixelCnt;
        /*
         * Decompress the symbol image and push it to the display.
         * We don't need to use a dedicated framebuffer (an array for the symbol image).
         * It's much more efficient to use a temporary buffer to store a chunk of the decompressed
         * frame and send it immediately as a data part of the frame, with a size equal to the chunk size.
         */
#define BLOCK_SIZE    254
        uint32_t blockCnt = (framePixelCnt + BLOCK_SIZE - 1) / BLOCK_SIZE;
        uint32_t i = 0;
        uint32_t n = 0;
        uint32_t tempPixelCnt = (framePixelCnt > chunkPixelCnt) ? chunkPixelCnt : framePixelCnt;
        for (uint32_t k = 0; k < blockCnt; k++) {
            uint32_t tempBlokSize = (k == (blockCnt - 1)) ? (framePixelCnt % BLOCK_SIZE) : BLOCK_SIZE;
            for (uint32_t m = 0; m < tempBlokSize; m++) {
                if (m == symbol.frame[i]) {
                    // set text color
                    memBuff[n++] = text.fontColorMsb;
                    memBuff[n++] = text.fontColorLsb;
                    i++;
                } else {
                    // set background color
                    memBuff[n++] = text.backColorMsb;
                    memBuff[n++] = text.backColorLsb;
                }
                if (n == tempPixelCnt * sizeof(GuiColor565)) {
                    cb.putPixels565((GuiColor565 *)memBuff, tempPixelCnt);
                    framePixelCnt -= tempPixelCnt;
                    tempPixelCnt = (framePixelCnt > chunkPixelCnt) ? chunkPixelCnt : framePixelCnt;
                    n = 0;
                }
            }
        }
        if (n != 0) {
            cb.putPixels565((GuiColor565 *)memBuff, tempPixelCnt);
        }

        text.x += symbol.w;
    }

    return GUI_STATUS_OK;
}

GuiStatus guiAddImage(GuiImg *img, uint16_t x, uint16_t y)
{
    if (img == NULL) {
        return GUI_STATUS_ERROR_NULL;
    }
    if (img->buff == NULL) {
        return GUI_STATUS_ERROR_NULL;
    }

    if (x + img->w > scrW) {
        return GUI_STATUS_ERROR_X;
    }
    if (y + img->h > scrH) {
        return GUI_STATUS_ERROR_Y;
    }
    
    cb.setRegion(x, y, img->w, img->h);
    cb.putPixels565(img->buff, img->size);

    return GUI_STATUS_OK;
}