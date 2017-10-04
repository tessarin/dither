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
#define PPM_HEADER 2

int ReadDataFromFile(DTImage *img, FILE *file);
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
    if (type == t_UNKNOWN) {
	fprintf(stderr, "Image file of unrecognized format.\n");
	return NULL;
    }

    /* initialize image and read pixels */
    DTImage *image = malloc(sizeof(DTImage));
    image->type = type;
    if (ReadDataFromFile(image, file)) {
	fprintf(stderr, "Failed to read image content.\n");
	return NULL;
    }

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

/* returns non-zero if error occurred reading the files */
int
ReadDataFromFile(DTImage *img, FILE *file)
{
    if (img->type == t_PPM) {
	/* simple format, done directly */
	fseek(file, PPM_HEADER, SEEK_SET);
	fscanf(file, " %d %d %*d ", &img->width, &img->height);
	img->pixels = malloc(sizeof(DTPixel) * img->width * img->height);
	img->resolution = img->width * img->height;

	for (int i = 0; i < img->resolution; i++)
	    fread(&img->pixels[i], sizeof(DTPixel), 1, file);
    }
    if (img->type == t_PNG) {
	/* create data and info structs */
	png_structp png = png_create_read_struct(
	    PNG_LIBPNG_VER_STRING, NULL, NULL, NULL
	);
	if (!png) return 1;

	png_infop info = png_create_info_struct(png);
	if (!info) return 2;

	/* in case an error occurs */
	if(setjmp(png_jmpbuf(png))) return 3;

	png_init_io(png, file);
	png_set_sig_bytes(png, HEADER_LEN);
	png_read_info(png, info);

	png_byte color_type, bit_depth;

	img->width = png_get_image_width(png, info);
	img->height = png_get_image_height(png, info);
	color_type = png_get_color_type(png, info);
	bit_depth = png_get_bit_depth(png, info);

	img->resolution = img->width * img->height;

	/* transform different types into 8bit RGB */
	if (bit_depth == 16) png_set_strip_16(png);

	if (color_type == PNG_COLOR_TYPE_PALETTE)
	    png_set_palette_to_rgb(png);

	if (color_type == PNG_COLOR_TYPE_GRAY && bit_depth < 8)
	    png_set_expand_gray_1_2_4_to_8(png);

	if (png_get_valid(png, info, PNG_INFO_tRNS))
	    png_set_tRNS_to_alpha(png);

	if (color_type & PNG_COLOR_MASK_ALPHA)
	    png_set_strip_alpha(png);

	if (color_type == PNG_COLOR_TYPE_GRAY ||
	    color_type == PNG_COLOR_TYPE_GRAY_ALPHA)
	    png_set_gray_to_rgb(png);

	png_read_update_info(png, info);

	img->pixels = malloc(sizeof(DTPixel) * img->width * img->height);

	/* check if data is realy 8-bit RGB */
	png_size_t rowSize = png_get_rowbytes(png, info);
	if (rowSize != img->width * sizeof(DTPixel))
	    return 4;

	/* setup array of pointers used by libpng to
	 * point to our own allocated memory */
	png_bytep *rowPointers = malloc(sizeof(png_bytep) * img->height);
	for (int i = 0; i < img->height; i++)
	    rowPointers[i] = (png_bytep)img->pixels + rowSize * i;

	png_read_image(png, rowPointers);

	/* finish reading and cleanup memory */
	png_read_end(png, info);
	png_destroy_read_struct(&png, &info, NULL);
	free(rowPointers);
    }

    return 0;
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
