/* 
 *  DTPalette.c
 *  dither Utility
 *
 *  Palette functions implementation.
 *
 */

#include <DTPalette.h>
#include <stdlib.h>

DTPalette
StandardPaletteBW()
{
    DTPalette palette;
    palette.size = 2;
    palette.colors = malloc(sizeof(DTPixel) * 2);
    palette.colors[0] = PixelFromRGB(0, 0, 0);
    palette.colors[1] = PixelFromRGB(0xFF, 0xFF, 0xFF);

    return palette;
}

DTPalette
StandardPaletteRGB()
{
    DTPalette palette;
    palette.size = 3;
    palette.colors = malloc(sizeof(DTPixel) * 3);
    palette.colors[0] = PixelFromRGB(0xFF, 0x00, 0x00);
    palette.colors[1] = PixelFromRGB(0x00, 0xFF, 0x00);
    palette.colors[2] = PixelFromRGB(0x00, 0x00, 0xFF);

    return palette;
}
