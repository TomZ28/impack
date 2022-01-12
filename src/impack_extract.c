#include "impack.h"
#include "impack_utils.h"

void extract_file(FILE *packed) {
    struct metadata *metadata;
    int name_len;
    long file_size, image_size;

    // Check if the image is packed first.
    if (!is_packed(packed)) {
        fprintf(stderr, "File is not packed\n");
        exit(EXIT_FAILURE);
    }

    // Extract the metadata from the image.
    metadata = get_metadata(packed);
    name_len = metadata->name_len;
    file_size = metadata->file_size;
    image_size = metadata->image_size;

    char *file_name = malloc(sizeof(char) * name_len);
    memcpy(file_name, metadata->file_name, name_len);

    // Extract the file from the image.
    extract(file_name, file_size, image_size, packed);

    printf("Extracted from image: file '%s' of size %ld\n", file_name, file_size);

    free(file_name);
}
