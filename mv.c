/* mv_mk.c */

#include "speedy.h"
#include "bb.h"
#include "chess.h"
#include "mv.h"

/* Data */
cstl_t cstl_tbl[64];

/* Functions */

/* cstl_mask() */
cstl_t cstl_mask(bb_t frm, bb_t to)
{
	bb_t mask;

	mask = (frm | to) & CSTL_MSK;
	return cstl_tbl[mask * CSTL_MGC >> 58];
}

/* mv_pckd_mk() */
void mv_mk(brd_t *old_brd, brd_t *brd, mv_t mv)
{
	mv_unpckd_mk(old_brd, brd, MV_GET_M(mv), MV_GET_C(mv), MV_GET_P(mv),
			MV_GET_F(mv), MV_GET_T(mv));
}

void mv_unpckd_mk(brd_t *old_brd, brd_t *brd, int pc_mv, int pc_cap,
		int pc_prom, int frm, int to)
{
	mv_bb_mk(old_brd, brd, pc_mv, pc_cap, pc_prom, MASK(frm), MASK(to));
}

/* mv_bb_mk() */
void mv_bb_mk(brd_t *old_brd, brd_t *brd, int pc_mv, int pc_cap,
		int pc_prom, bb_t frm, bb_t to)
{
	clr_t clr;
	pc_t pc;
	clr_t stm, sntm;

	stm = old_brd->stm;
	sntm = FLP_CLR(old_brd->stm);

	/* Copy old board piece positions. */
	FOR_EACH_CLR(clr)
		brd->clr_bb[clr] = old_brd->clr_bb[clr];

	FOR_EACH_PC(pc)
		brd->pc_bb[pc] = old_brd->pc_bb[pc];

	/* Remove captured piece. */
	brd->pc_bb[pc_cap] &= ~to;
	brd->clr_bb[sntm] &= ~to;

	/* Set moving piece. */
	brd->pc_bb[pc_mv] ^= frm;
	brd->clr_bb[stm] ^= frm;
	brd->pc_bb[pc_mv] |= to;
	brd->clr_bb[stm] |= to;

	/* Update castling rights. */
	brd->cstl = old_brd->cstl & cstl_mask(frm, to);

	/* Update side to move. */
	brd->stm = sntm;
}

bool_t mv_is_pseudo(brd_t *brd, mv_t mv)
{
	return BOOL_T;
}
