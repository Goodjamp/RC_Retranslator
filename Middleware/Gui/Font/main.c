#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>

#include "Font.h"

#define BLOCK_SIZE    254

extern const tFont Font24;

/*
 * Compression Font image
 */
int main(int vArgIn, char **vArg)
{
    if (vArgIn != 2) {
        return 1;
    }
    
    const char fileUrl[] = "../Font24_Compress.c";
    FILE *file = fopen(fileUrl, "w");

    fprintf(file, "#include <stdint.h>\n");
    fprintf(file, "#include <stdint.h>\n\n");
    fprintf(file, "#include \"Font.h\"\n\n");

    for (uint32_t k = 0; k < Font24.length; k++) {
        uint32_t fW = Font24.chars[k].image->w;
        uint32_t fH = Font24.chars[k].image->h;
        const uint8_t *frame = Font24.chars[k].image->frame;
        Utf8Code fCode = Font24.chars[k].code;
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
        fprintf(file, "const static tImage Font24_0x%x = {symbol_0x%x, %u, %u};\n\n", fCode, fCode, fW, fH);
    }

    fprintf(file, "static const tChar Font24_array[] = {\n");
    for (uint32_t k = 0; k < Font24.length; k++) {
        Utf8Code fCode = Font24.chars[k].code;
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
        fprintf(file, "\n    {0x%x, &Font24_0x%x},\n", fCode, fCode);
    }
    fprintf(file, "};\n\n");
    fprintf(file, "const tFont Font24 = { %u, Font24_array };\n", Font24.length);

    fclose(file);

    return 0;
}