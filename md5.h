#ifndef CRYPTATO_MD5_H
#define CRYPTATO_MD5_H

// Accepts a pointer to an arbirarily long sequence of bytes
// and the length in bits. Outputs 16-byte hash.
void md5(uint8_t* result, const uint8_t* data, int bits);

// Result as a string of 32 hexadecimal digits.
void md5_hex(char* result, const uint8_t* data, int bits);

#endif //CRYPTATO_MD5_H
