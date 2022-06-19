#ifndef STENOGRAPHY_CRYPTOGRAPHY_CIPHER_H
#define STENOGRAPHY_CRYPTOGRAPHY_CIPHER_H

int encrypt(char *algorithm, char *mode, char *pass, unsigned char *in,
            unsigned int in_len, unsigned char *out, unsigned int *out_len);

int decrypt(char *algorithm, char *mode, char *pass, unsigned char *in,
            unsigned int in_len, unsigned char *out, unsigned int *out_len);

#endif //STENOGRAPHY_CRYPTOGRAPHY_CIPHER_H
