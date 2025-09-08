#include <stdint.h>

#include "Font.h"

//extern const tFont Font16;
//extern const tFont Font20;
extern const tFont Font24;

static const tFont *const fontList[] = {
    //[GUI_FONT_SIZE_16] = &Font16,
    //[GUI_FONT_SIZE_20] = &Font20,
    [GUI_FONT_SIZE_24] = &Font24
};

GuiStatus guiFontGetSymbol(uint16_t utf8Code, GuiFontSize fontSize, Symbol *symbol)
{
    if (fontSize >= GUI_FONT_SIZE_CNT) {
        return GUI_STATUS_ERROR_FONT_SIZE;
    }

    const tFont *font = fontList[fontSize];
    
    uint32_t k = 0;
    
    for (; k < font->length; k++) {
        if (font->chars[k].code == utf8Code) {
            symbol->w = font->chars[k].image->w;
            symbol->h = font->chars[k].image->h;
            symbol->frame = font->chars[k].image->frame;
            break;
        }
    }

    return k == font->length ? GUI_STATUS_ERROR_SYMBOL_NOT_FOUND : GUI_STATUS_OK;
}