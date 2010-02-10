/* bb.h */

#ifndef _BB_H_
#define _BB_H_

/* Types */

typedef u64_t bb_t;

/* Masks */
#define MASK(s)		(1ull << (s))

/* Macros */
#define FOR_SQ_IN_BB(sq, bb)													\
	for (; (bb) != 0 && ((sq) = bsfq(bb), 1); (bb) &= (bb) - 1)

//#define _FOR_SQ_IN_BB(sq, bb, _bb)													\
//	for (bb_t _bb = (bb); _bb != 0 && ((sq) = lead_one(_bb), 1); \
//      (_bb) &= (_bb) - 1)
//
//#define FOR_SQ_IN_BB(sq, bb) _FOR_SQ_IN_BB((sq), (bb), _bb##__LINE__)

/* Functions */

static inline int bsfq(bb_t b)
{
	bb_t dummy;

	asm("bsfq %1, %0"
		: "=r" (dummy)
		: "r" (b));

	return dummy;
}

#endif
