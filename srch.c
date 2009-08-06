int alpha_beta(int alpha, int beta, int depth, int ply)
{
	struct atk_set_t atk[1];
	struct mv_slct_t slct[1];
	int best_score, score, legal_mvs;

	/* Init. */
	best_score = -VALUE_INF;
	legal_mvs = 0;

	/* Probe hash. */

	/* Null move. */

	/* IID. */

	/* Move loop. */
#define MV_SLCT_FN(mv) \
	do { \
		if (mv_make(mv)) { \
			legal_mvs++; \
			if (depth>0) {\
				if (legal_mvs>1) score = alpha_beta(-alpha-1, -alpha, depth-1, \
						ply+1); \
				else score = alpha_beta(-beta, -alpha, depth-1, ply+1); \
			} else score = quiesce(-beta, -alpha, ply+1); \
			mv_unmake(); \
			if (score > best_score) best_score = score; \
			if (score > alpha) alpha = score; \
			if (score >= beta) goto cut; \
		} \
	} while (0);

#include "move_selector.c"

cut:

	/* Store in hash. */

	return best_score;
}
