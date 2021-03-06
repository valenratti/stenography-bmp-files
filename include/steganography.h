#ifndef STEGANOGRAPHY_H
#define STEGANOGRAPHY_H

#include <stdint.h>
#include <stdio.h>
#include "bmp_parser.h"
#include "payload.h"
#include "cipher.h"
#include <string.h>
#include <stdlib.h>
#include "args.h"

#define PATTERNS 4  // 00 01 10 11

typedef struct steg_utils{
    uint8_t pixel_mask;
    uint8_t payload_mask;
    uint8_t shift;
    int type;
}steg_utils;

struct pattern_stats {
    unsigned int changed;
    unsigned int unchanged;
    char should_invert;
};

enum steganography_mode{LSB1, LSB4, LSB_IMPROVED};

void embed(bmp_file_p src, payload_p payload, char* mode);
payload_p extract(bmp_file_p src, struct args *args);

#endif
