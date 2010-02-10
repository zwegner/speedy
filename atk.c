/* atk.c */

#include <stdio.h>

#include "speedy.h"
#include "bb.h"
#include "chess.h"
#include "atk.h"

/* Data */

sq_t sq_ul[CLR_CT][SQ_CT];
sq_t sq_ur[CLR_CT][SQ_CT];

bb_t atk_pre_bb[SQ_CT][ATK_PRE_CT];

bb_t in_btwn_pre_bb[SQ_CT][SQ_CT];

sq_t sq_list[64][4][128][8]; /* 256K */
bb_t sq_mask[64][4]; /* 2K */
bb_t sq_magic[64][4]; /* 2K */

/* Functions */

/* in_btwn_bb() */
bb_t in_btwn_bb(sq_t from, sq_t to)
{
	return in_btwn_pre_bb[from][to];
}

/* print_bb() */
void print_bb(bb_t bb)
{
	int i;

	for (i = 56; i >= 0; i++) {
		printf("%i%c", (bb & MASK(i)) != 0, (i&7)==7?'\n':' ');
		if ((i&7)==7)i-=16;
	}
	printf("\n");
}

void init_magics(void)
{
	int i;
}

/* init_atks() */
void init_atks(void)
{
	bb_t mask;
	bb_t sq_bb;
	clr_t clr;
	sq_t sq;
	sq_t sq2;

	FOR_EACH_SQ(sq) {

		/* Pawns */

		FOR_EACH_CLR(clr) {
			sq_bb = MASK(sq);
			mask = CSH_UL(clr, sq_bb) | CSH_UR(clr, sq_bb);
			atk_pre_bb[sq][ATK_PRE_P(clr)] = mask;
		}

		sq_ur[CLR_W][sq] = sq+9;
		sq_ul[CLR_W][sq] = sq+7;
		sq_ur[CLR_B][sq] = sq-7;
		sq_ul[CLR_B][sq] = sq-9;

		/* Knights */

		sq_bb = MASK(sq);
		mask = 0;

		mask |= SH_UP(SH_UL(sq_bb));
		mask |= SH_LF(SH_UL(sq_bb));
		mask |= SH_UP(SH_UR(sq_bb));
		mask |= SH_RT(SH_UR(sq_bb));
		mask |= SH_DN(SH_DR(sq_bb));
		mask |= SH_RT(SH_DR(sq_bb));
		mask |= SH_DN(SH_DL(sq_bb));
		mask |= SH_LF(SH_DL(sq_bb));

		atk_pre_bb[sq][ATK_PRE_N] = mask;

#define INIT_ATK_BB(sh) \
		for (sq_bb = sh(MASK(sq)); sq_bb != 0; sq_bb = sh(sq_bb)) mask |= sq_bb;

		/* Bishops */

		mask = 0;

		INIT_ATK_BB(SH_UL);
		INIT_ATK_BB(SH_UR);
		INIT_ATK_BB(SH_DL);
		INIT_ATK_BB(SH_DR);

		atk_pre_bb[sq][ATK_PRE_B] = mask;

		/* Rooks */

		mask = 0;

		INIT_ATK_BB(SH_UP);
		INIT_ATK_BB(SH_DN);
		INIT_ATK_BB(SH_LF);
		INIT_ATK_BB(SH_RT);

		atk_pre_bb[sq][ATK_PRE_R] = mask;

		/* Kings */

		sq_bb = MASK(sq);
		mask = 0;
		mask |= SH_LF(sq_bb) | SH_RT(sq_bb);
		mask |= SH_UP(sq_bb | mask) | SH_DN(sq_bb | mask);
		atk_pre_bb[sq][ATK_PRE_K] = mask;

		/* In between masks */

		FOR_EACH_SQ(sq2)
			in_btwn_pre_bb[sq][sq2] = 0;

#define INIT_BTWN_BB(sh,inc) \
		do { \
			mask = 0; \
			for (sq2 = sq+(inc), sq_bb = sh(MASK(sq)); sq_bb != 0; \
					sq_bb = sh(sq_bb), sq2 += (inc)) { \
				in_btwn_pre_bb[sq][sq2] = mask; \
				mask |= sq_bb; \
			} \
		} while (0)

		INIT_BTWN_BB(SH_UL, 7);
		INIT_BTWN_BB(SH_UP, 8);
		INIT_BTWN_BB(SH_UR, 9);
		INIT_BTWN_BB(SH_RT, 1);
		INIT_BTWN_BB(SH_DR, -7);
		INIT_BTWN_BB(SH_DN, -8);
		INIT_BTWN_BB(SH_DL, -9);
		INIT_BTWN_BB(SH_LF, -1);

#undef INIT_BTWN_BB

		/* attack masks */
		sq_mask[sq][DIR_HORI] = RANK_MASK(RANK_OF(sq)) & ~MASK(sq);
		sq_mask[sq][DIR_VERT] = FILE_MASK(RANK_OF(sq)) & ~MASK(sq);

		diag = FILE_OF(sq) - RANK_OF(sq);
		if (diag > 0)
			sq_mask[sq][DIR_A1H8] = 0x0040201008040200ull >> (diag * 8);
		else
			sq_mask[sq][DIR_A1H8] = 0x0040201008040200ull << (-diag * 8);

		sq_mask[sq][DIR_A1H8] &= ~MASK(sq);

		diag = FILE_OF(sq) + RANK_OF(sq) - 7;
		if (diag > 0)
			sq_mask[sq][DIR_A8H1] = 0x0002040810204000ull << (diag * 8);
		else
			sq_mask[sq][DIR_A8H1] = 0x0002040810204000ull >> (-diag * 8);

		sq_mask[sq][DIR_A8H1] &= ~MASK(sq);
	}
}
