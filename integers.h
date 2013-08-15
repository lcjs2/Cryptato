#ifndef CRYPTATO_INTEGERS_H
#define CRYPTATO_INTEGERS_H
#include <stdint.h>
#include <stdlib.h>

#define BI_DEC_DIGITS BI_WORDS*10
#define BI_WORDS 128


size_t smax(size_t a, size_t b);
size_t smin(size_t a, size_t b);

// Little-endian
struct bigint {
	size_t num_words;
	uint32_t words[BI_WORDS];
};

typedef struct bigint bigint;

bigint newbigint();
int bit_count(bigint a);
bigint int_to_bigint(uint32_t a);
void bigint_to_string(char* output, bigint a);
bigint add(bigint a, bigint b);
bigint subtract(bigint a, bigint b);
bigint shift_left(bigint a, int n);
bigint divide_with_remainder(bigint a, bigint b, bigint* r);
bigint divide(bigint a, bigint b);

#endif // CRYPTATO_INTEGERS_H
