#include <string.h>
#include <openssl/evp.h>
#include <openssl/aes.h>
#include <openssl/des.h>
#include <openssl/err.h>

#include "include/cipher.h"


const EVP_CIPHER * get_cipher(char *algorithm, char *mode);


int encrypt(char *algorithm, char *mode, char *pass, unsigned char *in, unsigned int in_len,
            unsigned char *out, unsigned int *out_len) {

    const EVP_CIPHER *cipher = get_cipher(algorithm, mode);

    int key_len = EVP_CIPHER_key_length(cipher);
    int iv_len = EVP_CIPHER_iv_length(cipher);

    unsigned char key[key_len];
    unsigned char iv[iv_len];

    EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
    if(ctx == NULL)
        return -1;

    int final_len;
    int cipher_len = in_len + EVP_MAX_BLOCK_LENGTH;     // EVP_MAX_BLOCK_LENGTH = 32
    unsigned char cipher_text[cipher_len];

    EVP_BytesToKey(cipher, EVP_sha256(), NULL, (unsigned char *) pass, strlen(pass), 1, key, iv); // derives a key and IV from various parameters

    EVP_CIPHER_CTX_init(ctx);
    EVP_EncryptInit_ex(ctx, cipher, NULL, key, iv);

    if(!EVP_EncryptUpdate(ctx, cipher_text, &cipher_len, in, in_len))
        return -1;
    // TODO: chequear de aca para abajo
    if(!EVP_EncryptFinal_ex(ctx, cipher_text + cipher_len, &final_len))
        return -1;

    *out_len += final_len;

    memcpy(out, cipher_text, *out_len);

    EVP_CIPHER_CTX_cleanup(ctx); // FIXME: cleanup y free?
    EVP_CIPHER_CTX_free(ctx);

    return 0;
}


int decrypt(char *algorithm, char *mode, char *pass, unsigned char *in, unsigned int in_len,
            unsigned char *out, unsigned int *out_len) {

    const EVP_CIPHER *cipher = get_cipher(algorithm, mode);

    int key_len = EVP_CIPHER_key_length(cipher);
    int iv_len = EVP_CIPHER_iv_length(cipher);

    unsigned char key[key_len];
    unsigned char iv[iv_len];

    EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
    if(ctx == NULL)
        return -1;

    int final_len;
    int plain_len = in_len + EVP_MAX_BLOCK_LENGTH;     // EVP_MAX_BLOCK_LENGTH = 32
    unsigned char plain_text[plain_len];

    EVP_BytesToKey(cipher, EVP_sha256(), NULL, (unsigned char *) pass, strlen(pass), 1, key, iv); // derives a key and IV from various parameters

    EVP_CIPHER_CTX_init(ctx);
    EVP_DecryptInit_ex(ctx, cipher, NULL, key, iv);

    if(!EVP_DecryptUpdate(ctx, plain_text, &plain_len, in, in_len))
        return -1;
    // TODO: chequear de aca para abajo
    if(!EVP_DecryptFinal_ex(ctx, plain_text + plain_len, &final_len))
        return -1;

    *out_len += final_len;  // FIXME: why +=?

    memcpy(out, plain_text, *out_len);

    EVP_CIPHER_CTX_cleanup(ctx); // FIXME: cleanup y free?
    EVP_CIPHER_CTX_free(ctx);

    return 0;
}

enum algorithm str_to_enum_algo(char *algorithm) {
    if(strcmp(algorithm, "aes192") == 0)
        return AES192;

    if(strcmp(algorithm, "aes256") == 0)
        return AES256;

    if(strcmp(algorithm, "des") == 0)
        return DES;

    return AES128;  // default
}

enum mode str_to_enum_mode(char *mode) {
    if(strcmp(mode, "ecb") == 0)
        return ECB;

    if(strcmp(mode, "cfb") == 0)
        return CFB;

    if(strcmp(mode, "ofb") == 0)
        return OFB;

    return CBC;  // default
}

const EVP_CIPHER * get_cipher(char *algorithm, char *mode) {
    enum algorithm a = str_to_enum_algo(algorithm);
    enum mode m = str_to_enum_mode(mode);

    // default aes128 CBC mode
    switch(a) {
        case AES192:
            switch(m) {
                case ECB:
                    return EVP_aes_192_ecb();
                case CFB:
                    return EVP_aes_192_cfb8();
                case OFB:
                    return EVP_aes_192_ofb();
                case CBC:   // default
                default:    // cbc
                    return EVP_aes_192_cbc();
            }
        case AES256:
            switch(m) {
                case ECB:
                    return EVP_aes_256_ecb();
                case CFB:
                    return EVP_aes_256_cfb8();
                case OFB:
                    return EVP_aes_256_ofb();
                case CBC:   // default
                default:    // cbc
                    return EVP_aes_256_cbc();
            }
        case DES:
            switch(m) {
                case ECB:
                    return EVP_des_ecb();
                case CFB:
                    return EVP_des_cfb8();
                case OFB:
                    return EVP_des_ofb();
                case CBC:   // default
                default:    // cbc
                    return EVP_des_cbc();
            }
        case AES128: // default
        default:    // aes128
            switch(m) {
                case ECB:
                    return EVP_aes_128_ecb();
                case CFB:
                    return EVP_aes_128_cfb8();
                case OFB:
                    return EVP_aes_128_ofb();
                case CBC:   // default
                default:    // cbc
                    return EVP_aes_128_cbc();
            }
    }
    return EVP_aes_128_cbc();
}