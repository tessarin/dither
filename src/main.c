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
#include <DTImage.h>
#include <DTDither.h>

int
main(int argc, char ** argv)
{
    if (argc != 3) {
	fprintf(stderr, "Usage: %s input output", argv[0]);
	exit(EXIT_FAILURE);
    }

    DTImage *input = CreateImageFromFile(argv[1]);
    ApplyFloydSteinbergDither(input);
    WriteImageToFile(input, argv[2]);

    return 0;
}
