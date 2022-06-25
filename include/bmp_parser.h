#ifndef BMP_H
#define BMP_H

#include <stdint.h>
#include <stdio.h>
#include "payload.h"

struct __attribute__((__packed__)) bmp_header {
    uint16_t type; // 1 2
    uint32_t file_size; // 3 4 5 6
    uint16_t reserved1; // 7 8
    uint16_t reserved2; // 9 10
    uint32_t data_offset; // 11 12 13 14
    uint32_t header_size; // 15 16 17 18
    int32_t width; // 18 19 20 21
    int32_t height; // 22 23 24 25
    uint16_t planes; // 26 27
    uint16_t bits_per_pixel; // 28 29
    uint32_t compression; // 30 31 32 33
    uint32_t image_size; // 34 35 36 37
    int32_t x_pixels_per_m; // 38 39 40 41
    int32_t y_pixels_per_m; // 42 43 44 45
    uint32_t colors_used; // 46 47 48 49
    uint32_t important_colors; // 50 51 52 53
};

struct bmp_file {
    struct bmp_header* header;
    uint8_t* pixels;
};

typedef struct bmp_header * bmp_header_p; 
typedef struct bmp_file * bmp_file_p;

/**
 * @brief
 * Given a path for a BMP file,
 * parses it and returns a pointer to it's struct.
 * @param path
 * @param file_size
 * @return bmp_file_p
 */
bmp_file_p parse_bmp_file(char *path);
bmp_file_p hide_plain_file(payload_p plain_payload, char* mode_string);
bmp_file_p hide_encrypted_file(encrypted_payload_p encrypted_payload);


#endif
