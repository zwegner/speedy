/* brd.h */

#ifndef _BRD_H_
#define _BRD_H_

#include "speedy.h"
#include "bb.h"
#include "clr.h"
#include "mv.h"

/* Types */

struct brd_t
{
	bb_t pc_bb[6];
	bb_t clr_bb[2];
	clr_t stm;
	bb_t ep;
	cstl_t cstl;
};

/* Prototypes */


/* Data */


#endif
