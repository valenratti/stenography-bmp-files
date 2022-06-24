#ifndef BMP_H
#define BMP_H

#include <stdint.h>

struct bmp_header {
    uint_16_t type; // 1 2
    uint_32_t size; // 3 4 5 6
    uint_16_t reserved1; // 6 7
    uint_16_t reserved2; // 8 9
    uint_32_t data_offset; // 10 11 12 13
    uint_32_t size; // 14 15 16 17
    int32_t width; // 18 19 20 21
    int32_t height; // 22 23 24 25
    uint_16_t planes; // 26 27
    uint_16_t bits_per_pixel; // 28 29
    uint_32_t compression; // 30 31 32 33
    uint_32_t image_size; // 34 35 36 37
    int32_t x_pixels_per_m; // 38 39 40 41
    int32_t y_pixels_per_m; // 42 43 44 45
    uint_32_t colors_used; // 46 47 48 49
    uint_32_t important_colors; // 50 51 52 53
};

typedef struct bmp_header* bmp_header; 

struct bmp_file {
    bmp_header header;
    uint8_t* pixels;
}

uint8_t *read_file(char *path, long *fileSize);
#endif