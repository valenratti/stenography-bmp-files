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



uint8_t extract_byte_from_byte_array(uint8_t** data, steg_utils* utils){
    uint8_t data_bytes_needed = (8 / utils->type);
    uint8_t modified_byte = 0;
    for(int i=0; i<data_bytes_needed; i++){
        //We make xor between previous iteration and (byte & mask) << (shift - bits copied)
        modified_byte = modified_byte ^ ((*data)[i] & utils->pixel_mask) << (utils->shift - utils->type * i);
    }
    //ack that carrier byte has been used by displacing pointer so that next extraction uses from there
    *data += data_bytes_needed;
    return modified_byte;
}

uint32_t extract_size(uint8_t** data, steg_utils* utils){
    uint8_t byte_array[4];
    for(int i=0; i<4; i++){
        byte_array[i] = extract_byte_from_byte_array(data, utils);
    }
    return (byte_array[0] << 24) ^ (byte_array[1] << 16) ^ (byte_array[2] << 8) ^ byte_array[3];
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

payload_p extract(bmp_file_p src, struct args *arguments){
    enum steganography_mode mode_enum = get_mode_from_string(arguments->steg_algorithm);
    steg_utils* steg_utils = get_params_for_mode(mode_enum);
    if(strcmp(arguments->password, "") == 0) {
        //Not encrypted
        uint32_t embedded_size = extract_size(&src->pixels, steg_utils);
        payload_p payload = malloc(sizeof(struct payload));
        payload->file_size = embedded_size;
        payload->data = malloc(sizeof(uint8_t) * payload->file_size + 5);
        //Extract embedded data byte by byte into payload->data
        for(int i=0; i<payload->file_size; i++){
            payload->data[i] = extract_byte_from_byte_array(&src->pixels, steg_utils);
        }
        int k=0;
        payload->extension = malloc(10);
        int ended = 0;
        while(!ended){
            payload->extension[k] = extract_byte_from_byte_array(&src->pixels, steg_utils);
            if(payload->extension[k] == '\0')
                ended = 1;
            k++;
        }
        return payload;
    }
    return NULL;
}

void embed(bmp_file_p src, payload_p payload, char* mode){
    enum steganography_mode mode_enum = get_mode_from_string(mode);
    validate_file_fits(src->header->image_size, payload->total_size, mode_enum);
    steg_utils* steg_utils = get_params_for_mode(mode_enum);

    uint8_t payload_byte_iterator = payload->concat[0];
    long k=1;
    for(long i=0; i<payload->total_size* (8/steg_utils->type) ; i++){
        int bits_used_current_byte = (i * steg_utils->type) % 8;
        if(i != 0 && (i*steg_utils->type % 8) == 0){
            payload_byte_iterator = payload->concat[k++];
        }
//        int current = (payload_byte_iterator >> bits_used_current_byte);
        uint8_t pixel_byte = src->pixels[i];
//        uint8_t byte_mask = steg_utils->payload_mask ^ 0xFF;
//        src->pixels[i] = (pixel_byte & steg_utils->payload_mask) | (current & byte_mask);
        src->pixels[i] = ((payload_byte_iterator & (steg_utils->payload_mask >> bits_used_current_byte)) >> (steg_utils->shift - bits_used_current_byte)) ^ ((pixel_byte & steg_utils->pixel_mask) ^ pixel_byte);
    }
}
