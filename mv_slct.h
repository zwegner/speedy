/* mv_slct.h */

#ifndef _MV_SLCT_H_
#define _MV_SLCT_H_

#include "speedy.h"
#include "chess.h"

#define KILLER_CT (2)

typedef struct
{
	mv_t bad_caps[256];
	u8_t bad_cap_ct;

	mv_t kllr_mv[KILLER_CT];
} mv_slct_t;

#endif
