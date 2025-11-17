#include <stdint.h>

#include "Font.h"
#include "Gui.h"

//extern const tFont Font16;
//extern const tFont Font20;
//extern const tFont Font24;
//extern const tFont Font24_mono;
extern const tFont Font24_mono_1251;
//extern const tFont consolas24_1251;

static const tFont *const fontList[] = {
    //[GUI_FONT_SIZE_16] = &Font16,
    //[GUI_FONT_SIZE_20] = &Font20,
//    [GUI_FONT_SIZE_24] = &Font24
//,   [GUI_FONT_SIZE_24_mono] = &Font24_mono
    [GUI_FONT_SIZE_24_mono] = &Font24_mono_1251
//    [GUI_FONT_SIZE_24_mono] = &consolas24_1251
};

GuiStatus guiFontGetSymbol(uint16_t utf8Code, GuiFontSize fontSize, Symbol *symbol)
{
    if (fontSize >= GUI_FONT_SIZE_CNT) {
        return GUI_STATUS_ERROR_FONT_SIZE;
    }

    const tFont *font = fontList[fontSize];
    
    uint32_t  k = 0, k1 = 0, k2 = font->length - 1;
    GuiStatus  res = GUI_STATUS_ERROR_SYMBOL_NOT_FOUND;
/*    
    for (; k < font->length; k++) {
        if (font->chars[k].code == utf8Code) {
            symbol->w = font->chars[k].image->w;
            symbol->h = font->chars[k].image->h;
            symbol->frame = font->chars[k].image->frame;
            break;
        }
    }
*/
    
    while(1)
    {
        k = ( (k1 + k2) >> 1 );
        uint16_t code_k = font->chars[k].code;
        if (code_k == utf8Code) {
            symbol->w = font->chars[k].image->w;
            symbol->h = font->chars[k].image->h;
            symbol->frame = font->chars[k].image->frame;
            res = GUI_STATUS_OK;
            break;
        }
        if(k1 >= k2) break;
        if(code_k < utf8Code)
        { k1 = (k1 != k) ? k : k2;
        }
        else
        { k2 = (k2 != k) ? k : k1;
        }
    }
    //return symbol_found ? GUI_STATUS_OK : GUI_STATUS_ERROR_SYMBOL_NOT_FOUND;
    return res;
}
