/*
 *  DTPalette.h
 *  dither Utility
 *
 *  Palette generation, type and function declarations.
 *
 */

#ifndef DT_PALETTE
#define DT_PALETTE

#include <DTImage.h>

typedef struct {
    int size;
    DTPixel *colors;
} DTPalette;

DTPalette *StandardPaletteBW(int size);
DTPalette *StandardPaletteRGB();

DTPixel FindClosestColorFromPalette(DTPixel pixel, DTPalette *palette);

#endif
