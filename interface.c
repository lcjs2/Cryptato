#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include "md5.h"
#include "aes.h"



int main(int argc, char** argv)
{
	uint8_t block[16];
	memset(block,0,sizeof(block));

	uint8_t key[16];
	memset(key,0,sizeof(key));

	uint8_t result[16];
	memset(result,0,sizeof(result));
	
	aes_encode_block(result, block, key, 16);

	for(int i=0;i<16;i++) printf("%02x ",result[i]);
	printf("\n");

	aes_decode_block(block, result, key, 16);

	for(int i=0;i<16;i++) printf("%02x ",block[i]);
	printf("\n");

	return 0;
}
