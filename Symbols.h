#ifndef SYMBOLS_H
#define SYMBOLS_H

#include <stdint.h>

typedef struct {
    const uint8_t width;
    const uint8_t height;
    const uint16_t *data;
} FontDef;

extern FontDef Font_Custom;
extern FontDef Font_7x10;

const uint16_t* GetFontData(char c, FontDef* font);

#endif
