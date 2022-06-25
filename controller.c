#include "include/controller.h"

void embed_using_args(struct args args){
    bmp_file_p bmp_file =  parse_bmp_file(args.bitmap_file);
    if(strcmp(args.password, "") != 0) {
        payload_p payload = generate_payload_from_file(args.in_file);
        uint8_t* encrypted_bytes = malloc(payload->total_size * 2);
        unsigned int encrypted_size = 0;
        int encrypted_result = encrypt(args.cipher_algorithm, args.mode, args.password,
                                       payload->concat, payload->total_size, encrypted_bytes + sizeof(uint32_t), &encrypted_size);
        uint8_t size_in_big_endian[4] = {(uint32_t) encrypted_size >> 24, (uint32_t) encrypted_size >> 16, (uint32_t) encrypted_size >> 8, (uint32_t) encrypted_size};
        memcpy(encrypted_bytes, size_in_big_endian, 4);
        payload->concat = encrypted_bytes;
        payload->total_size = encrypted_size + sizeof(uint32_t);
        embed(bmp_file, payload, args.steg_algorithm);
    }else{
        payload_p plain_payload = generate_payload_from_file(args.in_file);
        embed(bmp_file, plain_payload, args.steg_algorithm);
        uint8_t* result = malloc(bmp_file->header->file_size);
        memcpy(result, bmp_file->header, (size_t) sizeof(struct bmp_header));
        memcpy(result + sizeof(struct bmp_header), bmp_file->pixels, bmp_file->header->image_size);
        FILE *file = fopen(args.out_file, "w");
        if (file == NULL) {
            printf("Error when creating new file");
            return;
        }
        size_t write_result = fwrite(result, 1, bmp_file->header->file_size, file);
        fclose(file);
    }
}

void extract_using_args(struct args args){
    bmp_file_p bmp_file =  parse_bmp_file(args.bitmap_file);
    payload_p payload = extract(bmp_file, &args);
    FILE *file = fopen(strcat(args.out_file,payload->extension), "w");
    fwrite(payload->data, 1, payload->file_size, file);
    fclose(file);
}
