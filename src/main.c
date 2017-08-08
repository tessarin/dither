/* 
 *  main.c
 *  dither Utility
 *
 *  Created on August 2017 by Cesar Tessarin.
 *  Rewritten and expanded from the 2010 version.
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <DTImage.h>
#include <DTDither.h>
#include <DTPalette.h>

DTPalette *PaletteForIdentifier(char *s);

int
main(int argc, char ** argv)
{
    char *paletteID = NULL;
    char *inputFile, *outputFile;
    int c;

    opterr = 0;

    while ((c = getopt(argc, argv, "p:")) != -1) {
	switch (c) {
	    case 'p':
		paletteID = optarg;
		break;
	    case '?':
		fprintf(stderr, "Ignoring unknown option: -%c\n", optopt);
	}
    }

    // check if there is still two arguments remaining, for i/o
    if (argc - optind != 2) {
	fprintf(stderr, "Usage: %s [-p palette[.size]] input output\n", argv[0]);
	return 1;
    }

    // all arguments ok
    inputFile = argv[optind];
    outputFile = argv[optind+1];

    DTImage *input = CreateImageFromFile(inputFile);
    if (input == NULL) return 2;

    DTPalette *palette = PaletteForIdentifier(paletteID);
    if (palette == NULL) return 3;

    ApplyFloydSteinbergDither(input, palette);
    WriteImageToFile(input, outputFile);

    return 0;
}

DTPalette *
PaletteForIdentifier(char *str)
{
    if (str == NULL) return StandardPaletteRGB();

    char *name, *sizeStr;
    char *sep = ".";
    name = strtok(str, sep);
    sizeStr = strtok(NULL, sep);
    int size = 0;

    // if size was inserted, transform to int and check if is valid
    if (sizeStr) {
	size = (int)strtol(sizeStr, (char **)NULL, 10);
	if (size <= 0) {
	    fprintf(stderr, "Invalid palette size, aborting.\n");
	    return NULL;
	}
    }

    // RGB
    if (strcmp(name, "rgb") == 0) {
	if (size) fprintf(stderr, "Ignored palette size.\n");
	return StandardPaletteRGB();
    }

    if (strcmp(name, "bw") == 0) {
	return StandardPaletteBW();
    }

    // unknown palette
    fprintf(stderr, "Unrecognized palette identifier, aborting.\n");
    return NULL;
}
