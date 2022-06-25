#include "include/controller.h"

void embed_using_args(struct args args){
    bmp_file_p bmp_file =  parse_bmp_file(args.bitmap_file);
    int encrypt = 0;
    if(encrypt){
        printf("Logic to encrypt");
    }else{
        payload_p plain_payload = generate_payload_from_file(args.in_file);
        embed(bmp_file, plain_payload, args.out_file, args.steg_algorithm);
        uint8_t* result = malloc(bmp_file->header->file_size);
        memcpy(result, bmp_file->header, (size_t) sizeof(struct bmp_header));
        memcpy(result + (size_t) sizeof(struct bmp_header), bmp_file->pixels, bmp_file->header->image_size);
        FILE *file = fopen(args.out_file, "w");
        if (file == NULL) {
            printf("Error when creating new file");
            return;
        }
        fwrite(result, bmp_file->header->file_size, 1, file);
        fclose(file);
    }
}
