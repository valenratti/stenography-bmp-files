#ifndef STEGANOGRAPHY_H
#define STEGANOGRAPHY_H
#include <stdint.h>
#include <stdio.h>
#include "bmp_parser.h"
#include "payload.h"
#include <string.h>
#include <stdlib.h>

typedef struct steg_utils{
    uint8_t pixel_mask;
    uint8_t payload_mask;
    uint8_t shift;
    int type;
}steg_utils;

enum steganography_mode{LSB1, LSB4, LSB_IMPROVED};
void embed(bmp_file_p src, payload_p payload, char *dest, char* mode);

#endif
