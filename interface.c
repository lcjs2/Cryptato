#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

#include "md5.h"
#include "aes.h"
#include "integers.h"


// Read a string of hexadecimal digits
void hex_to_byte(uint8_t* result, const char* data, int bytes)
{
	char byte[3];
	byte[2]='\n';
	for(int i=0;i<bytes;i++)
	{
		memcpy(byte, data+2*i, 2);
		*(result++)=strtol(byte, NULL, 16);
	}
	return;
}

// Output bytes as string of hex digits
void byte_to_hex(char* output, const uint8_t* data, int bytes)
{
	for(int i=0;i<bytes;i++)
	{
		sprintf(output, "%02x",*data);
		output+=2; data++;
	}
	return;
}

int main(int argc, char** argv)
{
	int a;
	bigint total=int_to_bigint(1);
	char output[BI_DEC_DIGITS+1];
	//for(;;)
	//{
		//scanf("%i",&a);
		a=atoi(argv[1]);
		total=shift_left(total, a);
		bigint_to_string(output, total);
		printf("%s\n",output);
		//printf("Total: %s\n", output);
		//printf("Words in total: %i\n",(int)total.num_words);
		//printf("Bits in total: %i\n",bit_count(total));
		//printf("words: ");
		//for(int i=5;i>=0;i--) printf("%lu ",(unsigned long)total.words[i]);
		//printf("\n");
	//}
	return 0;
}
