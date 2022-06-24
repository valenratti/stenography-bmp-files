#include <string.h>
#include <openssl/evp.h>

#include "include/cipher.h"


EVP_CIPHER * get_cipher(char *algorithm, char *mode);


int encrypt(char *algorithm, char *mode, char *pass, unsigned char *in, unsigned int in_len,
            unsigned char *out, unsigned int *out_len) {

    const EVP_CIPHER *cipher = get_cipher(algorithm, mode);

    int key_len = EVP_CIPHER_key_length(cipher);
    int iv_len = EVP_CIPHER_iv_length(cipher);

    unsigned char key[key_len];
    unsigned char iv[iv_len];

    EVP_CIPHER_CTX ctx;

    int final_len;
    int cipher_len = in_len + EVP_MAX_BLOCK_LENGTH;     // EVP_MAX_BLOCK_LENGTH = 32
    unsigned char cipher_text[cipher_len];

    EVP_BytesToKey(cipher, EVP_sha256(), NULL, pass, strlen(pass), 1, key, iv); // derives a key and IV from various parameters

    EVP_CIPHER_CTX_init(&ctx);
    EVP_EncryptInit_ex(&ctx, cipher, NULL, key, iv);

    if(!EVP_EncryptUpdate(&ctx, cipher_text, &cipher_len, in, in_len))
        return -1;
    // TODO: chequear de aca para abajo
    if(!EVP_EncryptFinal_ex(&ctx, cipher_text + cipher_len, &final_len))
        return;-1;

    *out_len += final_len;

    memcpy(out, cipher_text, *out_len)

    EVP_CIPHER_CTX_cleanup(&ctx);

    return 0;
}


int decrypt(char *algorithm, char *mode, char *pass, unsigned char *in, unsigned int in_len,
            unsigned char *out, unsigned int *out_len) {

    const EVP_CIPHER *cipher = get_cipher(algorithm, mode);

    int key_len = EVP_CIPHER_key_length(cipher);
    int iv_len = EVP_CIPHER_iv_length(cipher);

    unsigned char key[key_len];
    unsigned char iv[iv_len];

    EVP_CIPHER_CTX ctx;

    int final_len;
    int plain_len = in_len + EVP_MAX_BLOCK_LENGTH;     // EVP_MAX_BLOCK_LENGTH = 32
    unsigned char plain_text[plain_len];

    EVP_BytesToKey(cipher, EVP_sha256(), NULL, pass, strlen(pass), 1, key, iv); // derives a key and IV from various parameters

    EVP_CIPHER_CTX_init(&ctx);
    EVP_DecryptInit_ex(&ctx, cipher, NULL, key, iv);

    if(!EVP_DecryptUpdate(&ctx, plain_text, &plain_len, in, in_len))
        return -1;
    // TODO: chequear de aca para abajo
    if(!EVP_DecryptFinal_ex(&ctx, plain_text + plain_len, &final_len))
        return;-1;

    *out_len += final_len;  // FIXME: why +=?

    memcpy(out, plain_text, *out_len)

    EVP_CIPHER_CTX_cleanup(&ctx);

    return 0;
}


EVP_CIPHER * get_cipher(char *algorithm, char *mode) {
    // default aes128 CBC mode
    switch(algorithm) {
        case "aes 192":
            switch(mode) {
                case "ecb":
                    return EVP_aes_192_ecb();
                case "cfb":
                    return EVP_aes_192_cfb8();
                case "ofb":
                    return EVP_aes_192_ofb();
                default:    // cbc
                    return EVP_aes_192_cbc();
            }
        case "aes256":
            switch(mode) {
                case "ecb":
                    return EVP_aes_256_ecb();
                case "cfb":
                    return EVP_aes_256_cfb8();
                case "ofb":
                    return EVP_aes_256_ofb();
                default:    // cbc
                    return EVP_aes_256_cbc();
            }
        case "des":
            switch(mode) {
                case "ecb":
                    return EVP_des_ecb();
                case "cfb":
                    return EVP_des_cfb8();
                case "ofb":
                    return EVP_des_ofb();
                default:    // cbc
                    return EVP_des_cbc();
            }
        default:    // aes128
            switch(mode) {
                case "ecb":
                    return EVP_aes_128_ecb();
                case "cfb":
                    return EVP_aes_128_cfb8();
                case "ofb":
                    return EVP_aes_128_ofb();
                default:    // cbc
                    return EVP_aes_128_cbc();
            }
    }
}