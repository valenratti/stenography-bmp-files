#include "include/steganography.h"

void embed_lsbi(bmp_file_p src, payload_p payload);
payload_p extract_lsbi(bmp_file_p src, steg_utils* steg_utils, struct args *arguments);

void validate_file_fits(uint32_t size_src, uint32_t size_payload, enum steganography_mode mode){
    // FIXME: no seria size_payload + 5 + 4 ? porq tamb necesita espacio para el file size y la extension
    if(mode == LSB1){
        if(size_src / 8 < size_payload){
            printf("File to be carried does not fit in bmp src.");
            exit(1);
        }
    }
    else if(mode == LSB4){
        if(size_src / 2 < size_payload){
            printf("File to be carried does not fit in bmp src.");
            exit(1);
        }
    }
    else if(mode == LSB_IMPROVED) {
        if(size_src / 8 < (size_payload + PATTERNS)){    // necesita 4 bytes extra para el patron
            printf("File to be carried does not fit in bmp src.");
            exit(1);
        }
    }
}

enum steganography_mode get_mode_from_string(char* mode_string){
    if(strcmp(mode_string, "LSB1") == 0)
        return LSB1;
    if(strcmp(mode_string, "LSB4") == 0)
        return LSB4;
    if(strcmp(mode_string, "LSBI") == 0)
        return LSB_IMPROVED;

    printf("Algoritmo de esteganografía inválido");
    exit(1);
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
    if(mode == LSB1 || mode == LSB_IMPROVED){
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

    if(mode_enum == LSB_IMPROVED)
        return extract_lsbi(src, steg_utils, arguments);

    uint32_t embedded_size = extract_size(&src->pixels, steg_utils);
    payload_p payload = malloc(sizeof(struct payload));
    payload->file_size = embedded_size;
    payload->data = malloc(sizeof(uint8_t) * payload->file_size + 5);
    //Extract embedded data byte by byte into payload->data
    for(int i=0; i<payload->file_size; i++){
        payload->data[i] = extract_byte_from_byte_array(&src->pixels, steg_utils);
    }

    if(strcmp(arguments->password, "") == 0) {
        //Not encrypted
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
    }else{
        //Encrypted
        uint8_t* decrypted_result = malloc(payload->file_size * sizeof(uint8_t) * 2);
        unsigned int decrypted_length = 0;
        decrypt(arguments->cipher_algorithm, arguments->mode, arguments->password, payload->data, payload->file_size, decrypted_result, &decrypted_length);
        uint32_t plain_file_length = (decrypted_result[0] << 24) ^ (decrypted_result[1] << 16) ^ (decrypted_result[2] << 8) ^ decrypted_result[3];
        decrypted_result+= sizeof(uint32_t);
        free(payload->data);
        payload->data = calloc(sizeof(uint8_t) * plain_file_length, sizeof (uint8_t));
        for(int i=0; i<plain_file_length; i++){
            payload->data[i] = decrypted_result[i];
        }
        payload->extension = calloc(10, sizeof(uint8_t));
        strcpy(payload->extension, (char*) decrypted_result+plain_file_length);
        return payload;
    }
}

void embed(bmp_file_p src, payload_p payload, char* mode){
    enum steganography_mode mode_enum = get_mode_from_string(mode);
    validate_file_fits(src->header->image_size, payload->total_size, mode_enum);
    steg_utils* steg_utils = get_params_for_mode(mode_enum);

    if(mode_enum == LSB_IMPROVED) {
        embed_lsbi(src, payload);
        return;
    }

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

void embed_lsbi(bmp_file_p src, payload_p payload) {
    struct pattern_stats patterns[PATTERNS] = {0};

    int n = PATTERNS, j;   // dejamos 4 bytes libres para luego guardar el patron -> LSB1(patron)
    uint8_t byte, pixel, current_pattern, lsb_data, lsb_pixel;

    for(int i = 0; i < payload->total_size; i++) {
        byte = payload->concat[i];

        // para cada byte a ocultar necesitamos 8 bytes del portador
        for(j = 0; j < 8; j++) {
            pixel = src->pixels[n];
            current_pattern = (pixel >> 1) & 3;    // segundo y tercer LSB
            lsb_data = (byte >> (7 - j)) & 1;
            lsb_pixel = pixel & 1;

            if(lsb_data == lsb_pixel)
                patterns[current_pattern].unchanged++;
            else
                patterns[current_pattern].changed++;

            // LSB1
            src->pixels[n++] = pixel & 0xFE | lsb_data;
        }
    }

    // almacenamos el patron con LSB1
    for(int i = 0; i < PATTERNS; i++) {
        pixel = src->pixels[i];
        patterns[i].should_invert = patterns[i].changed > patterns[i].unchanged;
        src->pixels[i] = pixel & 0xFE | patterns[i].should_invert;
    }

    // inversion, si corresponde
    // recorremos nuevamente los pixels (solo en los cuales se almaceno data)
    // recordar que se dejaron los primeros 4 bytes para guardar el patron
    for(int i = PATTERNS; i < payload->total_size * 8 + PATTERNS; i++) {
        pixel = src->pixels[i];
        current_pattern = (pixel >> 1) & 3;    // segundo y tercer LSB
        if(patterns[current_pattern].should_invert)
            src->pixels[i] ^= 1;    // invierte
    }
}

payload_p extract_lsbi(bmp_file_p src, steg_utils* steg_utils, struct args *arguments) {
    struct pattern_stats patterns[PATTERNS] = {0};
    int j;
    uint8_t pixel, current_pattern, byte, lsb;

    // patron utilizado
    for(int i = 0; i < PATTERNS; i++)
        patterns[i].should_invert = src->pixels[i] & 1;

    src->pixels += PATTERNS;    // movemos el puntero
    uint8_t byte_array[4] = {0};
    for(int i = 0; i < 4; i++) {
        byte = 0;
        for (j = 0; j < 8; j++) {
            pixel = src->pixels[ i * 8 + j];
            current_pattern = (pixel >> 1) & 3;

            if (patterns[current_pattern].should_invert)
                pixel ^= 1;

            lsb = pixel & 1;
            byte |= lsb << (7 - j);
        }
        byte_array[i] = byte;
    }
    uint32_t embedded_size = (byte_array[0] << 24) ^ (byte_array[1] << 16) ^ (byte_array[2] << 8) ^ byte_array[3];
    src->pixels += sizeof(uint32_t) * 8;
    payload_p payload = malloc(sizeof(struct payload));
    payload->file_size = embedded_size;
    payload->data = malloc(sizeof(uint8_t) * payload->file_size + 5);


    // i = 0 porque ya se saltearon los bytes del patron y los bytes del tamaño del archivo
    for(int i = 0; i < payload->file_size; i++) {
        byte = 0;
        for (j = 0; j < 8; j++) {
            pixel = src->pixels[ i * 8 + j];
            current_pattern = (pixel >> 1) & 3;

            if (patterns[current_pattern].should_invert)
                pixel ^= 1;

            lsb = pixel & 1;
            byte |= lsb << (7 - j);
        }
        payload->data[i] = byte;
    }

    src->pixels += payload->file_size * 8;    // movemos el puntero

    // de aca para abajo es igual para los 3 metodos
    if(strcmp(arguments->password, "") == 0) {
        //Not encrypted
        int k=0;
        payload->extension = malloc(10);
        int ended = 0;
        while(!ended){
            byte = 0;
            for (j = 0; j < 8; j++) {
                pixel = src->pixels[ k * 8 + j];
                current_pattern = (pixel >> 1) & 3;

                if (patterns[current_pattern].should_invert)
                    pixel ^= 1;

                lsb = pixel & 1;
                byte |= lsb << (7 - j);
            }
            payload->extension[k] = byte;
            if(payload->extension[k] == '\0')
                ended = 1;
            k++;
        }
    }
    else {
        //Encrypted
        uint8_t* decrypted_result = malloc(payload->file_size * sizeof(uint8_t) * 2);
        unsigned int decrypted_length = 0;
        decrypt(arguments->cipher_algorithm, arguments->mode, arguments->password, payload->data, payload->file_size, decrypted_result, &decrypted_length);
        uint32_t plain_file_length = (decrypted_result[0] << 24) ^ (decrypted_result[1] << 16) ^ (decrypted_result[2] << 8) ^ decrypted_result[3];
        decrypted_result+= sizeof(uint32_t);
        free(payload->data);
        payload->data = calloc(sizeof(uint8_t) * plain_file_length, sizeof (uint8_t));
        for(int i=0; i<plain_file_length; i++){
            payload->data[i] = decrypted_result[i];
        }
        payload->extension = calloc(10, sizeof(uint8_t));
        strcpy(payload->extension, (char*) decrypted_result+plain_file_length);
    }

    return payload;
}
