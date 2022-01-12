#include <getopt.h>
#include <stdio.h>

/**
 * Packs the image file 'image' with the contents of 'file'.
 */
void pack_image(const char *file_name, FILE *file, FILE *image);

/**
 * Extracts a file stored in the image file 'packed'.
 */
void extract_file(FILE *packed);
