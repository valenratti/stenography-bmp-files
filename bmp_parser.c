#include "include/bmp_parser.h"
#include <stdint.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

bmp_file_p parse_bmp_file(char *path) {
    FILE *file = fopen(path, "rb");
    if(file == NULL){
        printf("Provided file does not exist.");
    }

    bmp_file_p bmp_file = (bmp_file_p) malloc(sizeof(struct bmp_file));
    bmp_file->header = (bmp_header_p) malloc(sizeof(struct bmp_header));
    //Read header into header pointer
    size_t header_read = fread(bmp_file->header, sizeof(struct bmp_header), 1, file);
    if(header_read != 1){
        printf("An error occurred when attempting to parse header.");
        exit(1);
    }

    //Allocate enough memory for all the bmp data.
    size_t file_length_in_bytes = bmp_file->header->image_size;
    bmp_file->pixels = (uint8_t*) malloc(file_length_in_bytes);
    //Read pixels into pixels pointer, file pointer should be reading to beginning of data
    size_t pixels_read = fread(bmp_file->pixels, file_length_in_bytes, 1, file);
    if(pixels_read != 1){
        printf("An error ocurred when attempting to parse pixels.");
        exit(1);
    }

    fclose(file);
    return bmp_file;
}

uint8_t* get_byte_array_from_payload(payload_p payload){
    uint8_t* byte_array = malloc(sizeof(uint32_t) + payload->file_size + strlen(payload->extension));
    *byte_array = payload->file_size;
    memcpy(byte_array + sizeof(uint32_t), payload->data, payload->file_size);
    memcpy(byte_array + sizeof(uint32_t) + payload->file_size , payload->extension, strlen(payload->extension) + 1);
    return byte_array;
}

//bmp_file_p hide_plain_file(payload_p payload, char* mode_string){
//    uint8_t* byte_array = get_byte_array_from_payload(payload);
//    enum steganography_mode mode = (enum steganography_mode) NULL;
//    if(strcmp(mode_string, "LSB1") == 0){
//        mode = LSB1;
//    }else if(strcmp(mode_string, "LSB4") == 0){
//        mode = LSB4;
//    }else if(strcmp(mode_string, "LSBI") == 0){
//        mode = LSB_IMPROVED;
//    }else{
//        printf("Invalid steganography mode");
//        exit(1);
//    }
//}


//bmp_file_p hide_encrypted_file(encrypted_payload_p encrypted_payload){
//
//}
