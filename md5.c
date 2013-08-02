#include <stdio.h>
#include <stdint.h>
#include <string.h>

// T[i] is the integer part of 2^32 * sin(i)
const uint32_t T[64] = {
0xd76aa478, 0xe8c7b756, 0x242070db, 0xc1bdceee,
0xf57c0faf, 0x4787c62a, 0xa8304613, 0xfd469501,
0x698098d8, 0x8b44f7af, 0xffff5bb1, 0x895cd7be,
0x6b901122, 0xfd987193, 0xa679438e, 0x49b40821,
0xf61e2562, 0xc040b340, 0x265e5a51, 0xe9b6c7aa,
0xd62f105d, 0x02441453, 0xd8a1e681, 0xe7d3fbc8,
0x21e1cde6, 0xc33707d6, 0xf4d50d87, 0x455a14ed,
0xa9e3e905, 0xfcefa3f8, 0x676f02d9, 0x8d2a4c8a,
0xfffa3942, 0x8771f681, 0x6d9d6122, 0xfde5380c,
0xa4beea44, 0x4bdecfa9, 0xf6bb4b60, 0xbebfbc70,
0x289b7ec6, 0xeaa127fa, 0xd4ef3085, 0x04881d05,
0xd9d4d039, 0xe6db99e5, 0x1fa27cf8, 0xc4ac5665,
0xf4292244, 0x432aff97, 0xab9423a7, 0xfc93a039,
0x655b59c3, 0x8f0ccc92, 0xffeff47d, 0x85845dd1,
0x6fa87e4f, 0xfe2ce6e0, 0xa3014314, 0x4e0811a1,
0xf7537e82, 0xbd3af235, 0x2ad7d2bb, 0xeb86d391};

// S is the number of bits to shift on each round.
// I have no idea how S was chosen.
const int S[64] = {
7, 12, 17, 22,  7, 12, 17, 22,  7, 12, 17, 22,  7, 12, 17, 22,
5,  9, 14, 20,  5,  9, 14, 20,  5,  9, 14, 20,  5,  9, 14, 20,
4, 11, 16, 23,  4, 11, 16, 23,  4, 11, 16, 23,  4, 11, 16, 23,
6, 10, 15, 21,  6, 10, 15, 21,  6, 10, 15, 21,  6, 10, 15, 21};

// Which word of the data to add each round
int word_choice(int i)
{
	switch(i/16)
	{
		case 0: return i;
		case 1: return (5*i+1)%16;
		case 2: return (3*i+5)%16;
		case 3: return (7*i)%16;
	}
	return 0;
}

void initialise(uint32_t* state)
{
	state[0] = 0x67452301;
	state[1] = 0xefcdab89;
	state[2] = 0x98badcfe;
	state[3] = 0x10325476;
	return;
}

uint32_t F(uint32_t X, uint32_t Y, uint32_t Z)
{
	// Returns (X and Y) or (not X and Z)
	return (X&Y)|((~X)&Z);
}

uint32_t G(uint32_t X, uint32_t Y, uint32_t Z)
{
	// Returns (X and Z) or (Y and not Z)
	return (X&Z)|(Y&(~Z));
}

uint32_t H(uint32_t X, uint32_t Y, uint32_t Z)
{
	// Returns X xor Y xor Z
	return X^Y^Z;
}

uint32_t I(uint32_t X, uint32_t Y, uint32_t Z)
{
	// Returns Y xor (X or not Z)
	return Y^(X|(~Z));
}

uint32_t bitmasher(uint32_t* state, int i)
{
	switch (i/16)
	{
		case 0: return F(state[1],state[2],state[3]);
		case 1: return G(state[1],state[2],state[3]);
		case 2: return H(state[1],state[2],state[3]);
		case 3: return I(state[1],state[2],state[3]);
	}
	return 0;
}

uint32_t left_shift(uint32_t val, int s)
{
	return (val<<s)|(val>>(32-s));
}

// Update the state by processing a 512-bit block
// (comprising sixteen 32-bit words)
// State consists of four 32-bit words
void md5_process_block(uint32_t* state, uint32_t* block)
{
	/*printf("Processing block:\n");
	for(int i=0;i<16;i+=4)
	{
		for(int j=0;j<4;j++) printf("%08x ",block[i+j]);
		printf("\n");
	}*/
	
	uint32_t start_state[4];
	memcpy(start_state, state, 16);
	// 64 rounds, each of which alters the first state word
	// and then cycles the four state words
	for(int i=0;i<64;i++)
	{
		// On each round, take state[0] and add:
		// a combination of other states (using F, G, H or I),
		// a certain word of the data, block[word_choice(i)],
		// and a constant, T[i].
		uint32_t val = state[0] + bitmasher(state, i) 
			+ block[word_choice(i)] + T[i];
				
		// Shift this value bitwise by a varying amount,
		// add state[1], and assign the result to state[0].
		state[0]=state[1] + left_shift(val, S[i]);
		
		// Cycle state words for next round
		uint32_t temp = state[0];
		state[0]=state[3];
		state[3]=state[2];
		state[2]=state[1];
		state[1]=temp;		
	}
	// Add  to the values from last round
	for(int i=0;i<4;i++) state[i]+=start_state[i];
	return;
}

// Read a little-endian 32-bit word from four octets
uint32_t word_from_octets(const uint8_t* data)
{
	return ((uint32_t)data[0])
		|(((uint32_t)data[1])<<8)
		|(((uint32_t)data[2])<<16)
		|(((uint32_t)data[3])<<24);
}

// Read 32-bit word from an arbirary number of bits,
// adding a 1 at the end	
uint32_t partial_word_from_octets(const uint8_t* data, int bits)
{
	uint8_t d[4];
	memset(d, 0, sizeof(d));
	for(int i=0;i<=(bits-1)/8;i++) d[i]=data[i];
	int offset = bits%8;
	if(offset==0)
	{
		d[bits/8]=0x80;
	} else {
		int n = (bits-1)/8;
		d[n]=((d[n]>>(8-offset))<<(8-offset));
		d[n]=d[n]|(1<<(7-offset));
	}
	return word_from_octets(d);
}
		
void md5(uint8_t* result, const uint8_t* data, uint64_t bits)
{
	uint32_t length[2];
	length[0]=(uint32_t)bits;
	length[1]=(uint32_t)(bits>>32);
	
	// Break input into 512-bit blocks (sixteen 32-bit words)
	uint32_t block[16];
	uint32_t state[4];
	initialise(state);

	while(bits>=512)
	{
		for(int i=0;i<16;i++)
		{
			block[i]=word_from_octets(data);
			data+=4;
		}
		bits-=512;
		md5_process_block(state, block);
	}

	// Read as many complete words as we can
	memset(block,0,sizeof(block));
	int words=0;
	while(bits>=32)
	{
		block[words++]=word_from_octets(data);
		data+=4;
		bits-=32;
	}
	// Read the last few bits, and add a 1
	block[words++]=partial_word_from_octets(data, bits);

	// Fill the last two words in this block with the length,
	// starting a new block if necessary
	if(words>14)
	{
		md5_process_block(state, block);
		memset(block,0,sizeof(block));
	}
	memcpy(block+14, length, 8);
	md5_process_block(state, block);

	for(int i=0;i<16;i++)
	{
		result[i]=(state[i/4]>>(8*i))%256;
	}	
	return;
}

void md5_hex(char* result, uint8_t* data, int bits)
{
	uint8_t byte_result[16];
	// Output hex
	for(int i=0;i<16;i++)
	{
		sprintf(result+(2*i),"%02x",byte_result[i]);
	}
	return;
}

/*int main(int argc, char** argv)
{
	if(argc<2)
	{
		printf("Provide string to hash as argument.\n");
		return 1;
	}
	
	char result[32];
	md5_hex(result, (uint8_t*)argv[1], strlen(argv[1])*8);
	puts(result);
	
	return 0;
}*/
