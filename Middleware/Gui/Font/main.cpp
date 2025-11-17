#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>

#include <string.h>

#include <string>

#include "Font.h"

#define BLOCK_SIZE    254

struct font_ref_t
{
    const tFont * font;
    const char *  name;
};

extern const tFont Font20;
extern const tFont Font24;
extern const tFont Font24_mono;
extern const tFont Font24_mono_1251;
extern const tFont consolas24;
extern const tFont consolas24_1251;

font_ref_t  fonts[] =
{
#define DEFINE_FONT(name)  { &name, #name }
  DEFINE_FONT(Font20) ,
  DEFINE_FONT(Font24) ,
  DEFINE_FONT(Font24_mono) ,
  DEFINE_FONT(Font24_mono_1251) ,
  DEFINE_FONT(consolas24) ,
  DEFINE_FONT(consolas24_1251) ,
  { 0, 0 },
#undef DEFINE_FONT
};

/*
 * Compression Font image
 */
int main(int vArgIn, char **vArg)
{
    const char * font_name_str = "Font24";
    if (vArgIn != 2) {
        //return 1;
    }
    else
    {
        font_name_str = vArg[1];
    }
    
    std::string  font_name;
    font_name += font_name_str;
    font_name += "_Compress.c";

    printf("creating: %s\n", font_name.c_str());
   
    int font_idx = -1;
    int font_idx_end = sizeof(fonts)/sizeof(font_ref_t);
   
    for(int i = 0;  i < font_idx_end;  i++)
    {
        if(fonts[i].font == 0) break;
        if(fonts[i].name == 0) break;
        if(strncmp(font_name_str, fonts[i].name, 1024) != 0)
        { continue;
        }
        font_idx = i;
    }
    if(font_idx < 0  ||  font_idx >= font_idx_end)
    {
        printf("can't find font %s\n", font_name_str);
        return 1;
    }
    FILE *file = fopen(font_name.c_str(), "w");
    if(file == NULL)
    {
        printf("can't create file %s\n", font_name.c_str());
        return 1;
    }

    const tFont & font = *fonts[font_idx].font;


    fprintf(file, "#include <stdint.h>\n");
    fprintf(file, "#include <stdint.h>\n\n");
    fprintf(file, "#include \"Font.h\"\n\n");

    for (uint32_t k = 0; k < font.length; k++) {
        uint32_t fW = font.chars[k].image->w;
        uint32_t fH = font.chars[k].image->h;
        const uint8_t *frame = font.chars[k].image->frame;
        Utf8Code fCode = font.chars[k].code;
        uint8_t code[2];
        uint32_t frameSize = fW * fH;
        if ((fCode >> 8) & 0xFF) {
            code[0] = (fCode >> 8) & 0xFF;
            code[1] = fCode & 0xFF;
        } else {
            code[0] = fCode & 0xFF; 
            code[1] = 0;
        }
        fprintf(file, "// Symbol \"");
        fwrite(code, 1, code[1] ? 2 : 1, file);
        fprintf(file, "\"\n");
        fprintf(file, "static const uint8_t symbol_0x%x[] = {\n", fCode);

        for (uint8_t i = 0; i < ((frameSize + BLOCK_SIZE - 1) / BLOCK_SIZE ); i++) {
            bool blockClear = true;
            for (uint32_t m = i * BLOCK_SIZE, n = 0; m < frameSize && n < BLOCK_SIZE; m++, n++) {
                if (m % fW == 0) {
                    fprintf(file, "// \n");
                }
                if (frame[m * 2] == 0x00) {
                    fprintf(file, "%u, ", n);
                    blockClear = false;
                }
            }
            if (blockClear) {
                fprintf(file, "0xFF, ");
            }
        }
        fprintf(file, "};\n\n");
        fprintf(file, "const static tImage %s_0x%x = {symbol_0x%x, %u, %u};\n\n", font_name_str, fCode, fCode, fW, fH);
    }

    fprintf(file, "static const tChar %s_array[] = {\n", font_name_str);
    for (uint32_t k = 0; k < font.length; k++) {
        Utf8Code fCode = font.chars[k].code;
        uint8_t code[2];
        if ((fCode >> 8) & 0xFF) {
            code[0] = (fCode >> 8) & 0xFF;
            code[1] = fCode & 0xFF;
        } else {
            code[0] = fCode & 0xFF; 
            code[1] = 0;
        }
        fprintf(file, "// Symbol \"");
        fwrite(code, 1, code[1] ? 2 : 1, file);
        fprintf(file, "\"\n    {0x%x, &%s_0x%x},\n", fCode, font_name_str, fCode);
    }
    fprintf(file, "};\n\n");
    fprintf(file, "const tFont %s = { %u, %s_array };\n", font_name_str, font.length, font_name_str);

    fclose(file);

    printf("done.\n");

    return 0;
}