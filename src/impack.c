#include "impack.h"
#include "impack_utils.h"

int main(int argc, char **argv) {
    char *usage_pack = "./impack <path to image> <path to file>";
    char *usage_extract = "./impack -e <path to image>";

    if (argc != 3) {
        fprintf(stderr, "Usage:\n\t%s\n\t%s\n", usage_pack, usage_extract);
        exit(EXIT_FAILURE);
    }

    int opt, img_arg;
    int extract_flag = 0;

    while ((opt = getopt(argc, argv, "e:")) != -1) {
        switch (opt) {
        case 'e':
            extract_flag = 1;
            break;
        default:
            exit(EXIT_FAILURE);
        }
    }

    // The image file's argument number changes based on whether an optarg exists or not.
    if (extract_flag)
        img_arg = 2;
    else
        img_arg = 1;

    // Check if the image exists.
    if (access(argv[img_arg], R_OK) != 0) {
        fprintf(stderr, "Image file does not exist\n");
        exit(EXIT_FAILURE);
    }

    // Check if the file extension is supported.
    char *image_name = get_file_name(argv[img_arg]);
    if (!is_image(image_name)) {
        fprintf(stderr, "%s is not a supported image type\n", image_name);
        exit(EXIT_FAILURE);
    }

    if (extract_flag) { // Extract
        FILE *image = open_file(argv[img_arg], "rb");
        extract_file(image);
        fclose(image);
    }
    else {              // Pack
        FILE *image = open_file(argv[img_arg], "a");

        // Check if the pack file exists.
        if (access(argv[2], R_OK) != 0) {
            fprintf(stderr, "Pack file does not exist\n");
            exit(EXIT_FAILURE);
        }
        FILE *file = open_file(argv[2], "rb");
        char *file_name = get_file_name(argv[2]);
        pack_image(file_name, file, image);
        fclose(image);
        fclose(file);
    }

    return 0;
}