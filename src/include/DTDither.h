/*
 *  DTDither.h
 *  dither Utility
 *
 *  Dithering methods declarations
 *
 */

#ifndef DT_DITHER
#define DT_DITHER

#include <DTImage.h>
#include <DTPalette.h>

void ApplyFloydSteinbergDither(DTImage *image, DTPalette *palette);

#endif
