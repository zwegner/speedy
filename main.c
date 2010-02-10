/* main.c */

#include <stdio.h>

#include "speedy.h"
#include "chess.h"
#include "init.h"
#include "atk.h"
#include "mv_slct.h"

int perft(brd_t *brd, int depth)
{
	brd_t chld_brd[1];
	mv_slct_t slct[1];
	int r;

	if (depth <= 0)
		return 1;

	/* Move loop. */
	r = 0;

#define MV_SLCT_FN(mv) \
		mv_mk(brd, chld_brd, mv); \
			r += perft(chld_brd, depth - 1); \

#include "mv_slct.c"

#undef MV_SLCT_FN

	return r;
}

int main(void)
{
	brd_t brd[1], chld_brd[1];
	mv_slct_t slct[1];

	init();

	parse_fen(brd,"rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
	//"3r4/p6k/4p1rp/1pp1Pp2/2pN3R/4PQ1R/P2K2PP/1q6 b - - 0 1");
	display_board(brd);

	slct->hash_mv = 0;

	printf("sq(28)="SQ_FMT"\n",SQ_ARGS(28));
	/* Move loop. */
#define MV_SLCT_FN(mv) \
		printf("got move: m=%o f="SQ_FMT" t="SQ_FMT" m=%i c=%i p=%i\n", mv,\
				SQ_ARGS(MV_GET_F(mv)), \
				SQ_ARGS(MV_GET_T(mv)),\
				MV_GET_M(mv),\
				MV_GET_C(mv),\
				MV_GET_P(mv));\

#include "mv_slct.c"

#undef MV_SLCT_FN

	int x;
	for (x = 1; x < 5; x++)
		printf("p[%i]=%i\n", x, perft(brd, x));
}
