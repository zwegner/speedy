/* brd.h */

#include "speedy.h"
#include "bb.h"
#include "clr.h"
#include "mv.h"

struct brd_t
{
	bb_t piece_bb[6];
	bb_t occ_bb[2];
	clr_t stm;
	bb_t ep;
	cstl_t cstl;
};
