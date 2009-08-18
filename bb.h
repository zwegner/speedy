/* bb.h */

#ifndef _BB_H_
#define _BB_H_

typedef u64_t bb_t;

static inline int bsfq(bb_t b)
{
	bb_t dummy;

	asm("bsfq %1, %0"
		: "=r" (dummy)
		: "r" (b));

	return dummy;
}

#endif
