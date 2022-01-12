#ifndef IMPACK_UTILS
#define IMPACK_UTILS

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <libgen.h>
#include <unistd.h>

#define MAX_NAME_LEN    255
#define BUF_SIZE        4096

#define NUM_EXTS        5

extern char *extensions[NUM_EXTS];  // The extensions/image types that impack supports.

struct metadata {
    char file_name[MAX_NAME_LEN];   // The name of the file to be stored
    int name_len;                   // The length of file_name
    long file_size;                 // The size of the file
    long image_size;                // The size of the image before being packed
};

/**
 * Given a path, returns the final component.
 */
char *get_file_name(char *path);

/**
 * Given a file name, returns whether it is an image or not.
 */
int is_image(char *file_name);

/**
 * Given a file name and a mode, attempts to open the file with
 * that mode. Returns an open file pointer.
 */
FILE *open_file(char *file_name, char *mode);

/**
 * Initializes the given struct metadata with the given values.
 */
void init_metadata(struct metadata *data, const char *file_name, unsigned long file_size, unsigned long image_size);

/**
 * Returns the size of the given image file in bytes.
 */
long get_size(FILE *file);

/**
 * Returns whether the given image file is packed or not.
 */
int is_packed(FILE *file);

/**
 * Given an image file, returns the metadata stored in it.
 */
struct metadata *get_metadata(FILE *file);

/**
 * Extract a file with name 'file_name' from the image file 'file'.
 * The file_size parameter is the size of the file to be extracted,
 * and 'offset' is the byte offset in 'file' to begin extraction from.
 */
void extract(char *file_name, long file_size, long offset, FILE *file);

/**
 * Appends 'size' bytes of content from 'src' to 'dest'.
 */
void append_to_file(FILE *src, FILE *dst, long size);

#endif