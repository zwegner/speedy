/* srch.c */

#include <stdio.h>

#include "speedy.h"
#include "chess.h"
#include "bb.h"
#include "atk.h"
#include "mv_slct.h"

int alpha_beta(brd_t *brd, int alpha, int beta, int depth, int ply)
{
	brd_t chld_brd[1];
	mv_slct_t slct[1];
	int s, lgl_mvs;

	/* Init. */
	lgl_mvs = 0;

	/* Generate attacks. */

	/* Probe hash. */

	/* Null move. */

	/* IID. */

	/* Move loop. */

	/*FOR SPLIT:---- goto LBL(left,__LINE__); \ */

#define PREFIX moves
#define MV_SLCT_FN(mv) \
	do { \
		if (mv_mk(brd, chld_brd, mv)) { \
			lgl_mvs++; \
			if (depth>0) {\
				if (lgl_mvs>1) s = alpha_beta(chld_brd, -alpha-1, -alpha, \
						depth-1, ply+1); \
				else s = alpha_beta(chld_brd, -beta, -alpha, depth-1, ply+1); \
			} else s = quiesce(chld_brd, -beta, -alpha, ply+1); \
			\
			if (s > alpha) alpha = s; \
			if (s >= beta) goto cut; \
		} \
	} while (0);

#include "mv_slct.c"

#undef PREFIX
#undef MV_SLCT_FN

cut:

	/* Store in hash. */

	return alpha;

	/* Leftover: collect unsearched moves. Happens on a split. */

	/* Move loop. */
#define PREFIX left
#define MV_SLCT_FN(mv) \
	do { \
		printf("mv=%i\n");\
	} while (0);

#include "mv_slct.c"

#undef PREFIX
#undef MV_SLCT_FN
}

int quiesce(brd_t *brd, int alpha, int beta, int ply)
{
	brd_t chld_brd[1];
	mv_slct_t slct[1];
	int s, lgl_mvs;

	/* Init. */
	lgl_mvs = 0;

	/* Probe hash. */

	/* Null move. */

	/* IID. */

	/* Move loop. */
#define MV_SLCT_FN(mv) \
	do { \
		if (mv_mk(brd, chld_brd, mv)) { \
			lgl_mvs++; \
			s = quiesce(brd, -beta, -alpha, ply+1); \
			if (s > alpha) alpha = s; \
			if (s >= beta) goto cut; \
		} \
	} while (0);

#include "mv_slct.c"

#undef MV_SLCT_FN

cut:

	/* Store in hash. */

	return alpha;
}
