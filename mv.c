/* mv_mk.c */

#include "speedy.h"
#include "bb.h"
#include "clr.h"
#include "mv.h"
#include "brd.h"

int cstl_mgc_idx(bb_t msk)
{
	return msk * cstl_mgc >> 58;
}

void mv_mk(brd_t *brd, int pc_mv, int pc_cap, bb_t frm, bb_t to)
{
	bb_t msk;
	clr_t sntm;

	sntm = FLP_CLR(brd->stm);

	/* Remove captured piece. */
	brd->pc_bb[pc_cap] &= ~to;
	brd->clr_bb[sntm] &= ~to;

	/* Set moving piece. */
	brd->pc_bb[pc_move] ^= frm;
	brd->clr_bb[brd->stm] ^= frm;
	brd->pc_bb[pc_move] |= to;
	brd->clr_bb[brd->stm] |= to;

	/* Update castling rights. */
	msk = (frm | to) & cstl_rghts_msk;
	brd->cstl &= cstl_tbl[castle_magic_idx(msk)];

	/* Update side to move. */
	brd->stm = sntm;
}
