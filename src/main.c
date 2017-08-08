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
#include <DTImage.h>
#include <DTDither.h>
#include <DTPalette.h>

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
	fprintf(stderr, "Usage: %s [-p palette] input output\n", argv[0]);
	exit(EXIT_FAILURE);
    }

    // all arguments ok
    inputFile = argv[optind];
    outputFile = argv[optind+1];

    DTImage *input = CreateImageFromFile(inputFile);
    if (input == NULL) return 1;

    ApplyFloydSteinbergDither(input, StandardPaletteRGB());
    WriteImageToFile(input, outputFile);

    return 0;
}
