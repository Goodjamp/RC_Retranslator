#ifndef __FONT_H__
#define __FONT_H__

#include <stdint.h>

#include "Gui.h"

typedef uint16_t Utf8Code;

typedef struct {
    const uint8_t *frame;
    uint32_t w;
    uint32_t h;
} tImage;

 typedef struct {
     Utf8Code code;
     const tImage *image;
} tChar;

 typedef struct {
     int length;
     const tChar *chars;
} tFont;

typedef struct {
    uint16_t w;
    uint16_t h;
    const uint8_t *frame;
} Symbol;

GuiStatus guiFontGetSymbol(Utf8Code utf8Code, GuiFontSize fontSize, Symbol *symbol);

#endif