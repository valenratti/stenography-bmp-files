#include "include/payload.h"
#include <stdint.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

uint32_t get_file_size(char* path) {
    FILE *file = fopen(path, "r");
    fseek(file, 0L, SEEK_END);
    uint32_t file_size = ftell(file);
    fclose(file);
    return file_size;
}

char* get_file_extension(char *path){
    char* file_extension;
    int i = 0;
    if(*path == '.'){
        i++;
    }
    while (path[i] != '.') i++;
    int size = strlen(path) - i;
    file_extension = calloc(10, sizeof(char));
    strcpy(file_extension, path+i);
    return file_extension;
}

payload_p generate_payload_from_file(char* path){
    FILE *file = fopen(path, "rb");
    if(file == NULL){
        printf("Provided file to create a payload does not exist.");
    }
    char* file_extension = get_file_extension(path);
    payload_p plain_payload = (payload_p) malloc(sizeof(struct payload));
    uint32_t file_size = get_file_size(path);
    plain_payload->extension = file_extension;
    plain_payload->file_size = file_size;
    plain_payload->data = malloc(file_size+1);
    size_t read_bytes = fread(plain_payload->data, 1, file_size, file);
    plain_payload->total_size = file_size + sizeof(uint32_t) + strlen(plain_payload->extension) + 1;
    plain_payload->concat = calloc(plain_payload->total_size, sizeof(uint8_t));
    uint8_t size_in_big_endian[4] = {plain_payload->file_size >> 24, plain_payload->file_size >> 16, plain_payload->file_size >> 8, plain_payload->file_size};
//    memcpy(plain_payload->concat, size_in_big_endian, 4);
    plain_payload->concat[0] = plain_payload->file_size >> 24;
    plain_payload->concat[1] = plain_payload->file_size >> 16;
    plain_payload->concat[2] = plain_payload->file_size >> 8;
    plain_payload->concat[3] = plain_payload->file_size;
    memcpy(plain_payload->concat + sizeof(uint32_t), plain_payload->data, sizeof(uint8_t ) * plain_payload->file_size);
    memcpy(plain_payload->concat + sizeof(uint32_t) + plain_payload->file_size, plain_payload->extension, strlen(plain_payload->extension) + 1);
    return plain_payload;
}
