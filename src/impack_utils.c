#include "impack_utils.h"

char *extensions[NUM_EXTS] = {".jpg", ".jpeg", ".png", ".pdf", ".bmp"};

char *get_file_name(char *path) {
    char *name = strrchr(path, '/');
    return name ? ++name : path;
}

int is_image(char *file_name) {
    char *ext;
    int ext_len;

    ext = strrchr(file_name, '.');
    ext_len = strlen(ext);

    if (!ext) return 0;
    
    // Check if the extension is in the list of supported extensions.
    for (int i = 0; i < NUM_EXTS; i++) {
        if (strncmp(ext, extensions[i], ext_len) == 0) {
            return 1;
        }
    }

    return 0;
}

FILE *open_file(char *file_name, char *mode) {
    FILE *fp = fopen(file_name, mode);
    if (!fp) {
        fprintf(stderr, "Could not open file: %s\n", file_name);
        exit(errno);
    }
    return fp;
}

void init_metadata(struct metadata *data, const char *file_name, unsigned long file_size, unsigned long image_size) {
    int name_len = strlen(file_name);
    if (name_len > MAX_NAME_LEN) {
        perror("File name too long");
        exit(ENAMETOOLONG);
    }

    memset(data->file_name, 0, MAX_NAME_LEN);
    memcpy(data->file_name, file_name, name_len);
    data->name_len = name_len;
    data->file_size = file_size;
    data->image_size = image_size;
}

long get_size(FILE *file) {
    if (fseek(file, 0, SEEK_END) < 0) {
        perror("Unable to get file size");
        exit(errno);
    }
    return ftell(file);
}

int is_packed(FILE *file) {
    char buf[6];
    fseek(file, -6, SEEK_END);
    fread(&buf, 6, 1, file);
    return strncmp(buf, "PACKED", 6) == 0;
}

struct metadata *get_metadata(FILE *file) {
    struct metadata *metadata;
    char buf[BUF_SIZE];

    // The metadata should be stored at the end of the image.
    fseek(file, -(sizeof(struct metadata) + 6), SEEK_END);
    if (fread(&buf, sizeof(struct metadata), 1, file) == 0) {
        perror("Could not read metadata from image");
        exit(EXIT_FAILURE);
    }
    metadata = (struct metadata *) buf;

    return metadata;
}

void extract(char *file_name, long file_size, long offset, FILE *file) {
    int bytes_read;
    char buf[BUF_SIZE];

    // First, check if the file already exists.
    if (access(file_name, R_OK) == 0) {
        fprintf(stderr, "File already exists\n");
        exit(EXIT_FAILURE);
    }

    // Create and open the file to be extracted.
    FILE *output = open_file(file_name, "wb");
    
    fseek(file, offset, SEEK_SET);  // Seek to the offset, from beginning of file

    while (file_size > 0) {
        if (file_size >= 4096)
            bytes_read = fread(&buf, sizeof(char), BUF_SIZE, file);
        else
            bytes_read = fread(&buf, sizeof(char), file_size, file);
        
        if (bytes_read == 0) {
            perror("Error reading from image file");
            exit(EXIT_FAILURE);
        }

        if (fwrite(&buf, sizeof(char), bytes_read, output) < bytes_read) {
            perror("Error while extracting file from image");
            exit(1);
        }

        file_size -= bytes_read;
    }

    if (fclose(output) != 0) {
        perror("Error closing extracted file");
        exit(EXIT_FAILURE);
    }
}

void append_to_file(FILE *src, FILE *dst, long size) {
    char buf[BUF_SIZE];
    int bytes_read;
    int total = 0;
    
    fseek(src, 0, SEEK_SET); // Read from start of src
    fseek(dst, 0, SEEK_END); // Write to end of dst
    
    while (total < size) {
        bytes_read = fread(&buf, sizeof(char), BUF_SIZE, src);
        if (bytes_read == 0) {
            perror("Error reading from file");
            exit(EXIT_FAILURE);
        }

        if (fwrite(&buf, sizeof(char), bytes_read, dst) < bytes_read) {
            perror("Error while writing to image file");
            exit(EXIT_FAILURE);
        }

        total += bytes_read;
    }
}
