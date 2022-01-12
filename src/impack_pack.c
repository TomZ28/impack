#include "impack.h"
#include "impack_utils.h"

void pack_image(const char *file_name, FILE *file, FILE *image) {
    struct metadata metadata;
    long file_size = get_size(file);
    long image_size = get_size(image);
    init_metadata(&metadata, file_name, file_size, image_size);

    // Append the contents of 'file' to 'image'.
    append_to_file(file, image, file_size);

    // Write the metadata to the end of 'image'.
    if (fwrite(&metadata, sizeof(struct metadata), 1, image) < 1) {
        perror("Error writing metadata to image");
        exit(EXIT_FAILURE);
    }

    // Write a string to 'image' to indicate it is packed.
    if (fwrite("PACKED", sizeof(char), 6, image) < 6) {
        perror("Error writing indicator to image");
        exit(EXIT_FAILURE);
    }
    
    printf("Packed image with: file '%s' of size %ld\n", file_name, file_size);
}
