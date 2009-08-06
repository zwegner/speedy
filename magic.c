#include <stdio.h>
#include <stdlib.h>

typedef unsigned long long bb_t;

bb_t castle = 0x9100000000000091;

int idx[64];

int castle_mask(bb_t mask)
{
	int c;

	c = 15;
	if (mask & 0x0000000000000090) // WK
		c &= ~1;
	if (mask & 0x0000000000000011) // WQ
		c &= ~2;
	if (mask & 0x9000000000000000) // BK
		c &= ~4;
	if (mask & 0x1100000000000000) // BQ
		c &= ~8;

	return c;
}

int search(bb_t mul)
{
	int i;
	int r;
	int c;
	int ret = -22;
	bb_t s = 0;

	for (i = 0; i < 64; i++)
		idx[i] = -1;

	do
	{
		r = (mul * s) >> 59 & 63; // r = magic idx
		c = castle_mask(s); // c = castling rights

		//printf("%#018llx = %i [%i]\n",s,c,r);

		if (idx[r] != -1 && idx[r] != c) // no match
		{
			ret = 0;
			goto done;
		}

		idx[r] = c;

		s = (s - castle) & castle;
		//printf("%#018llx - %i\n",castle);
	} while (s);

	// good
	for (i = 63; i >= 0; i--)
		if (idx[i] != -1)
			break;

		for (r = 0; r < 64; r++)
			if (idx[r] != -1)
				break;

		ret = i - r;
done:

	//getchar();
	return ret;
}

bb_t random_bb_t() {
	bb_t u1, u2, u3, u4;
	u1 = (bb_t)(random()) & 0xFFFF; u2 = (bb_t)(random()) & 0xFFFF;
	u3 = (bb_t)(random()) & 0xFFFF; u4 = (bb_t)(random()) & 0xFFFF;
	return u1 | (u2 << 16) | (u3 << 32) | (u4 << 48);
}

bb_t rand_64() {
	return random_bb_t() & random_bb_t() & random_bb_t();
}
const bb_t deBruijn = (0x03f79d71b4cb0a89);

const unsigned int deBruijnLookup[64] = // or unsigned char
{
    0,  1, 48,  2, 57, 49, 28,  3,
   61, 58, 50, 42, 38, 29, 17,  4,
   62, 55, 59, 36, 53, 51, 43, 22,
   45, 39, 33, 30, 24, 18, 12,  5,
   63, 47, 56, 27, 60, 41, 37, 16,
   54, 35, 52, 21, 44, 32, 23, 11,
   46, 26, 40, 15, 34, 20, 31, 10,
   25, 14, 19,  9, 13,  8,  7,  6,
};

// get next greater value with same number of one bits
bb_t snoob (bb_t x)
{
   bb_t smallest, ripple, ones;
   smallest = x & (0-x);
   ripple   = x + smallest;
   ones     = x ^ ripple;
   ones     = (ones >> 2) >> deBruijnLookup[(smallest*deBruijn) >> 58];
   return ripple | ones;
}

int main(void)
{
	int i, best, card;
	bb_t x, first;

	x = 0x0480000000000021;

	for (card = 3; card < 64; card++)
	{
		printf("card=%i\n",card);
		best=64;
		first = x = (1ull << card) - 1;
		printf("mask=%#018llx\n",x);
		do
		{
			i = search(x);
			if (i && i < best)
			{
				printf("%#018llx = %i\n",x,i);
				best = i;
			}
			x = snoob(x);
			//printf("mask=%#018llx\n",x);
		} while (x > first);
	}
}
