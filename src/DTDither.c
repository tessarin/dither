/*
 *  DTDither.c
 *  dither Utility
 *
 *  Dithering algorithm Implementation.
 *
 */

#include <DTDither.h>

typedef struct {
    short int r, g, b;
} DTDiff;

DTDiff CalculateDifference(DTPixel original, DTPixel new);
void ApplyDifference(DTPixel *pixel, DTDiff diff, int factor);
byte ByteCap(int n);

void
ApplyFloydSteinbergDither(DTImage *image, DTPalette palette)
{
    for (int i = 0; i < image->height; i++) {
	for (int j = 0; j < image->width; j++) {
	    DTPixel original = image->pixels[i*image->width + j];
	    DTPixel new = FindClosestColorFromPalette(original, palette);

	    DTDiff diff = CalculateDifference(original, new);

	    // disperse
	    if (j+1 < image->width)
		ApplyDifference( (
		    image->pixels + i*image->width + j + 1
		), diff, 7);
	    if (i+1 < image->height) {
		if (j-1) ApplyDifference( (
		    image->pixels + (i+1)*image->width + j - 1
		), diff, 3);
		ApplyDifference( (
		    image->pixels + (i+1)*image->width + j
		), diff, 5);
		if (j+1 < image->width) ApplyDifference( (
		    image->pixels + (i+1)*image->width + j + 1
		), diff, 1);
	    }

	    image->pixels[i*image->width + j] = new;
	}
    }
}

DTDiff
CalculateDifference(DTPixel original, DTPixel new)
{
    DTDiff diff;

    diff.r = original.r - new.r;
    diff.g = original.g - new.g;
    diff.b = original.b - new.b;

    return diff;
}

void
ApplyDifference(DTPixel *pixel, DTDiff diff, int factor)
{
    pixel->r = ByteCap(pixel->r + (diff.r * factor / 16));
    pixel->g = ByteCap(pixel->g + (diff.g * factor / 16));
    pixel->b = ByteCap(pixel->b + (diff.b * factor / 16));
}

byte
ByteCap(int num)
{
    if (num > 255) return 255;
    if (num < 0) return 0;

    return num;
}
