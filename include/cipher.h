#ifndef CIPHER_H
#define CIPHER_H

enum algorithm{AES128, AES192, AES256, DES};
enum mode{CBC, ECB, CFB, OFB};

int encrypt(char *algorithm, char *mode, char *pass, unsigned char *in,
            unsigned int in_len, unsigned char *out, unsigned int *out_len);

int decrypt(char *algorithm, char *mode, char *pass, unsigned char *in,
            unsigned int in_len, unsigned char *out, unsigned int *out_len);

#endif //CIPHER_H
