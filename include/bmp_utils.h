#include <stdint.h>

struct bmp_header {
    // Header 14 bytes
    unsigned char signature[2]; // 0 1
    int file_size; // 2 3 4 5
    unsigned char reserved[4]; // 6 7 8 9
    int data_offset; // 10 11 12 13
    // InfoHeader 40 bytes
    int size; // 14 15 16 17
    int width; // 18 19 20 21
    int height; // 22 23 24 25
    int planes; // 26 27
    int bits_per_pixel; // 28 29
    int compression; // 30 31 32 33
    int image_size; // 34 35 36 37
    int x_pixels_per_m; // 38 39 40 41
    int y_pixels_per_m; // 42 43 44 45
    int colors_used; // 46 47 48 49
    int important_colors; // 50 51 52 53
};

uint8_t *read_file(char *path, long *file_size);