/* srch.c */

#include "speedy.h"
#include "bb.h"
#include "clr.h"
#include "mv.h"

int alpha_beta(int alpha, int beta, int depth, int ply)
{
	struct atk_set_t atk[1];
	struct mv_slct_t slct[1];
	int s, lgl_mvs;

	/* Init. */
	lgl_mvs = 0;

	/* Probe hash. */

	/* Null move. */

	/* IID. */

	/* Move loop. */
#define MV_SLCT_FN(mv) \
	do { \
		if (mv_mk(mv)) { \
			lgl_mvs++; \
			if (depth>0) {\
				if (lgl_mvs>1) s = alpha_beta(-alpha-1, -alpha, depth-1, \
						ply+1); \
				else s = alpha_beta(-beta, -alpha, depth-1, ply+1); \
			} else s = quiesce(-beta, -alpha, ply+1); \
			mv_unmk(); \
			if (s > alpha) alpha = s; \
			if (s >= beta) goto cut; \
		} \
	} while (0);

#include "mv_slct.c"

cut:

	/* Store in hash. */

	return alpha;
}
