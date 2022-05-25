#include "bmp_utils.h"
#include <stdint.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>

/**
 * @brief 
 * Given a file, reads it and returns a pointer
 * in memory to the beggining of the file
 * @param path 
 * @param fileSize 
 * @return uint8_t* 
 */
uint8_t *readFile(char *path, long *fileSize) {
    FILE *bmpImage = fopen(path, "rb");
    fseek(bmpImage, 0, SEEK_END);
    *fileSize = ftell(bmpImage);
    rewind(bmpImage);
    uint8_t *fileData = malloc(sizeof(uint8_t) * (*fileSize));
    size_t readBytes = fread(fileData, sizeof(uint8_t), *fileSize, bmpImage);
    fclose(bmpImage);
    if (readBytes != *fileSize) {
        free(fileData);
        return NULL;
    }
    return fileData;
}