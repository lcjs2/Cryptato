#ifndef CRYPTATO_AES_H
#define CRYPTATO_AES_H

// Accepts a block of 16 bytes, a key, and the key length
// in bytes (16, 24 or 32). Outputs 16 bytes encoded.
void aes_encode_block(uint8_t* result, const uint8_t* data,
		const uint8_t* key, int key_size);

void aes_decode_block(uint8_t* result, const uint8_t* data,
		const uint8_t* key, int key_size);

#endif // CRYPTATO_AES_H
