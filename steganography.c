#include "include/steganography.h"

void validate_file_fits(uint32_t size_src, uint32_t size_payload, enum steganography_mode mode){
    if(mode == LSB1){
        if(size_src / 8 < size_payload){
            printf("File to be carried does not fit in bmp src.");
            exit(1);
        }
    }else if(mode == LSB4){
        if(size_src / 2 < size_payload){
            printf("File to be carried does not fit in bmp src.");
            exit(1);
        }
    }
}

enum steganography_mode get_mode_from_string(char* mode_string){
    if(strcmp(mode_string, "LSB1") == 0){
        return LSB1;
    }else if(strcmp(mode_string, "LSB4") == 0){
        return LSB4;
    }else if(strcmp(mode_string, "LSBI") == 0){
        return LSB_IMPROVED;
    }else{
        printf("Invalid steganography mode");
        exit(1);
    }
}

steg_utils* get_params_for_mode(enum steganography_mode mode ){
    steg_utils* steg_utils = malloc(sizeof(steg_utils));
    if(mode == LSB1){
        steg_utils->pixel_mask = 0x01;
        steg_utils->shift = 7;
        steg_utils->type = 1;
    }else if(mode == LSB4){
        steg_utils->pixel_mask = 0x0F;
        steg_utils->shift = 4;
        steg_utils->type = 4;
    }
    steg_utils->payload_mask = steg_utils->pixel_mask << steg_utils->shift;
    return steg_utils;
}

void embed(bmp_file_p src, payload_p payload, char *dest, char* mode){
    enum steganography_mode mode_enum = get_mode_from_string(mode);
    validate_file_fits(src->header->image_size, payload->file_size, mode_enum);
    steg_utils* steg_utils = get_params_for_mode(mode_enum);

    uint8_t payload_byte_iterator = payload->data[0];
    long k=1;
    for(long i=0; i<payload->file_size*steg_utils->type; i++){
        int bits_used_current_byte = (i * steg_utils->type) % 8;
        if(i != 0 && (i*steg_utils->type & 8) == 0){
            payload_byte_iterator = payload->concat[k++];
        }
        uint8_t pixel_byte = src->pixels[i];
        src->pixels[i] = ((payload_byte_iterator & (steg_utils->payload_mask >> bits_used_current_byte)) >>
                (steg_utils->shift - bits_used_current_byte)) ^ ((pixel_byte & steg_utils->pixel_mask) ^ pixel_byte);
    }
}
