/*
 *  DTDither.c
 *  dither Utility
 *
 *  Dithering algorithm Implementation.
 *
 */

#include <DTDither.h>

void
ApplyFloydSteinbergDither(DTImage *image)
{
    for (int i = 0; i < image->resolution; i++) {
	DTPixel pixel = image->pixels[i];

	image->pixels[i] = pixel;
    }
}
