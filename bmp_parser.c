#include "include/bmp_utils.h"
#include <stdint.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>

/**
 * @brief 
 * Given a file, reads it and returns a pointer
 * in memory to the beggining of the file
 * @param path 
 * @param file_size
 * @return uint8_t* 
 */
uint8_t *read_file(char *path, long *file_size) {
    FILE *bmp_image = fopen(path, "rb");

    fseek(bmp_image, 0, SEEK_END);

    *file_size = ftell(bmp_image);

    rewind(bmp_image);

    uint8_t *file_data = malloc(sizeof(uint8_t) * (*file_size));
    size_t read_bytes = fread(file_data, sizeof(uint8_t), *file_size, bmp_image);

    fclose(bmp_image);

    if (read_bytes != *file_size) {
        free(file_data);
        return NULL;
    }

    return file_data;
}