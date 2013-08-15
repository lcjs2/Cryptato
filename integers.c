#include <stdint.h>
#include <string.h>
#include <stdio.h>

#include "integers.h"

size_t smax(size_t a, size_t b){ return a>b?a:b;}
size_t smin(size_t a, size_t b){ return a<b?a:b;}

bigint newbigint()
{
	bigint a;
	a.num_words=0;
	memset(a.words, 0, sizeof(a.words));
	return a;
}

int bit_count(bigint a)
{
	if(a.num_words==0) return 0;
	uint32_t top=a.words[a.num_words-1];
	int bits=0;
	while(top>0)
	{
		top>>=1;
		bits++;
	}
	return bits+(32*(a.num_words-1));
}

bigint int_to_bigint(uint32_t a)
{
	bigint big_a=newbigint();
	big_a.words[0]=a;
	big_a.num_words=(big_a.words[0]>0)?1:0;
	return big_a;
}

void bigint_to_string(char* output, bigint a)
{
	if(a.num_words==0)
	{
		sprintf(output,"0");
		return;
	}
	bigint ten=int_to_bigint(10);
	bigint r;
	char buffer[BI_DEC_DIGITS];
	int len=0;
	while(a.num_words>0)
	{
		a=divide_with_remainder(a,ten, &r);
		sprintf(buffer+(len++),"%i",r.words[0]);
		printf("Dividing by 10\n");
		printf("Result: %lu %lu, remainder %i\n",a.words[1],a.words[0],r.words[0]);
		
	}
	//Reverse string
	for(int i=0;i<len/2;i++)
	{
		char temp=buffer[i];
		buffer[i]=buffer[len-i-1];
		buffer[len-i-1]=temp;
	}
	strcpy(output, buffer);
	return;
}

int is_g_or_geq(bigint a, bigint b, int if_eq)
{
	if(a.num_words>b.num_words) return 1;
	if(a.num_words<b.num_words) return 0;
	for(int i=a.num_words-1;i>=0;--i)
	{
		if(a.words[i]>b.words[i]) return 1;
		if(a.words[i]<b.words[i]) return 0;
	}
	return if_eq;
}		

int is_geq(bigint a, bigint b)
{
	return is_g_or_geq(a, b, 1);
}

int is_greater(bigint a, bigint b)
{
	return is_g_or_geq(a,b,0);
}

int is_less(bigint a, bigint b)
{
	return 1-is_g_or_geq(a,b,1);
}

int is_leq(bigint a, bigint b)
{
	return 1-is_g_or_geq(a,b,0);
}

bigint add(bigint a, bigint b)
{
	bigint c=newbigint();
	int carry=0;
	for(int i=0;i<smax(a.num_words, b.num_words);i++)
	{
		uint64_t d=(uint64_t)a.words[i]+(uint64_t)b.words[i];
		if(carry) d++;
		c.words[i]=(uint32_t)d;
		carry=(int)(d>>32);
	}
	c.num_words=smin(BI_WORDS,smax(a.num_words, b.num_words)+carry);
	return c;
};

bigint subtract(bigint a, bigint b)
{
	if(is_greater(b, a)) return int_to_bigint(0);
	if(b.num_words==0) return a;
	int borrow=0;
	size_t word_count=0;
	for(int i=0;i<b.num_words;i++)
	{
		a.words[i]-=borrow;
		if(b.words[i]<=a.words[i])
		{
			a.words[i]-=b.words[i];
			borrow=0;
		} else {
			b.words[i]-=a.words[i]+1;
			borrow=1;
			a.words[i]=~0;
			a.words[i]-=b.words[i];
		}
		// Keep track of highest non-zero word in result
		if(a.words[i]>0) word_count=i+1;
	}
	if(borrow && b.num_words<BI_WORDS) a.words[b.num_words]-=borrow;
	if(a.words[a.num_words-1]==0) a.num_words=word_count;
	return a;
}

bigint shift_left(bigint a, int n)
{
	if(n==0) return a;
	int w = n/32;
	bigint c = newbigint();
	c.num_words=a.num_words+w;
	for(int i=w;i<smin(BI_WORDS,c.num_words);i++) c.words[i]=a.words[i-w];
	n%=32;
	if(n==0) return c;
	uint32_t carry=0;
	for(int i=0;i<smin(BI_WORDS,c.num_words+1);i++)
	{
		// NB do not shift 32 bits (n=0)! This causes undefined behaviour
		uint32_t carry_from_this=c.words[i]>>(32-n);
		c.words[i]<<=n;
		c.words[i]+=carry;
		carry=carry_from_this;
	}
	if(c.num_words<BI_WORDS && c.words[c.num_words]>0) c.num_words++;
	return c;
}

bigint divide_with_remainder(bigint a, bigint b, bigint* r)
{
	if(b.num_words==0) {printf("Division by zero!"); return int_to_bigint(0);}
	bigint total=newbigint();
	bigint one=int_to_bigint(1);
	while(is_geq(a,b))
	{
		int d = bit_count(a)-bit_count(b)-1;
		// If a, b same length then we can subtract all of b
		// since we know a>=b
		if(d==-1) d=0;
		a=subtract(a, shift_left(b,d));
		total=add(total, shift_left(one, d));
	}
	if(r!=NULL) *r=a;
	return total;
}
	
	
