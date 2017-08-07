#include <stdio.h>
#include <stdlib.h>
#include <DTImage.h>

int
main(int argc, char ** argv)
{
    if (argc != 3) {
	fprintf(stderr, "Usage: %s input output", argv[0]);
	exit(EXIT_FAILURE);
    }

    DTImage *input = CreateImageFromFile(argv[1]);
    WriteImageToFile(input, argv[2]);

    return 0;
}
