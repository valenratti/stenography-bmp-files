#ifndef PAYLOAD_H
#define PAYLOAD_H

#include <stdint.h>
#include <stdio.h>

struct payload {
    uint32_t file_size;
    uint8_t* data;
    char* extension;
    uint8_t* concat;
    uint32_t total_size;
};

struct encrypted_payload {
    uint32_t encrypted_length;
    uint8_t* encrypted_data; // encrypt(real_size || data || extension)
};

typedef struct payload * payload_p;
typedef struct encrypted_payload * encrypted_payload_p;

payload_p generate_payload_from_file(char* path);
encrypted_payload_p generate_encrypted_payload_from_payload(payload_p payload);

#endif
