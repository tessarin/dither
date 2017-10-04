/*
 *  DTImage.c
 *  dither Utility
 *
 *  Implementation of image handling functions.
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <DTImage.h>
#include <png.h>

#define HEADER_LEN 8

void ReadPixelsFromFile(DTImage *img, FILE *file);
DTImageType IdentifyImageType(char *header);

DTImage *
CreateImageFromFile(char *filename)
{
    FILE *file = fopen(filename, "rb");
    if (file == NULL) {
	perror("Could not open image file");
	return NULL;
    }

    char header[HEADER_LEN];
    size_t read = fread(&header, 1, HEADER_LEN, file);
    if (read != HEADER_LEN) {
	fprintf(stderr, "Failed to read image header.\n");
	return NULL;
    }
    DTImageType type = IdentifyImageType(header);

    DTImage *image = malloc(sizeof(DTImage));

    fscanf(file, " %d %d %*d ", &image->width, &image->height);
    image->resolution = image->width * image->height;
    ReadPixelsFromFile(image, file);

    fclose(file);

    return image;
}

void
WriteImageToFile(DTImage *img, char *filename)
{
    FILE *file = fopen(filename, "w");
    if (file == NULL) {
	perror("Could not open output file");
	return;
    }

    fprintf(file, "P6\n%d %d\n255\n", img->width, img->height);
    for (int i = 0; i < img->resolution; i++)
	fwrite(&img->pixels[i], sizeof(DTPixel), 1, file);
}

DTPixel
PixelFromRGB(byte r, byte g, byte b)
{
    DTPixel pixel;
    pixel.r = r;
    pixel.g = g;
    pixel.b = b;

    return pixel;
}

void
ReadPixelsFromFile(DTImage *img, FILE *file)
{
    img->pixels = malloc(sizeof(DTPixel) * img->width * img->height);

    for (int i = 0; i < img->resolution; i++)
	fread(&img->pixels[i], sizeof(DTPixel), 1, file);
}

DTImageType
IdentifyImageType(char *header)
{
    if (!png_sig_cmp((png_const_bytep)header, 0, HEADER_LEN))
	return t_PNG;

    if (header[0] == 'P' && header[1] == '6')
	return t_PPM;

    return t_UNKNOWN;
}
