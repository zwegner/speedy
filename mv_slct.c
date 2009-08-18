/* mv_slct.c */

/* This file works as a move selector, but to make the code cleaner/faster
 * at the expense of some code size, we implement it as a seperate file
 * with a function defined as a macro before each inclusion. This generates
 * moves as a finite state machine. It basically works like Ken Thompson's
 * Belle in bitboards, but more complex. Since we don't want to deal with
 * maintaining the state of the generator in between calls, which besides
 * being messy also requires some extra calculations. So we call the
 * "child" functions directly from the move selector. But since we want to
 * make this code modular and usable in multiple locations, we end up with
 * this. In order to use the move selector, just define these macros:
 *
 * MV_SLCT_FN(mv): the function to call when a good move is found.
 * IS_QUIESCE: whether this selector is in quiescent search, so we should not
 * 				generate noncaptures/bad captures. We also use a "futility"
 * 				mask in case the selector is quiescent, to make this function
 * 				even faster.
 *
 * After these are defined, just include this file. It creates a function
 * with the given name, as described. The function takes an attack_set_t
 * and a move_selector_t as an argument.
 *
 * We try moves in the following manner:
 *
 * Hash move
 * Captures in MVV/LVA order, after testing with SEE
 * Killer moves 1/2 (if not quiescent)
 * Noncaptures
 * Bad captures (found during the MVV/LVA phase, tested in that order)
 *
 */

#ifdef IS_QUIESCE
#	define ADD_BAD_CAP(mv)
#else
#	define ADD_BAD_CAP(mv) slct->bad_caps[slct->bad_cap_count++] = mv
#endif

#define CAP_LOOP(pc) \
	FOR_SQ_IN_BB(atkr_sq, atkr_bb) { \
		mv_t mv = MV_FROM_FTMCP(atkr_sq, vctm_sq, pc, vctm_pc, 0); \
		if (mv_is_good(mv)) \
			MV_SLCT_FN(mv); \
	}

#define SLDR_CAP_LOOP(pc, dir) \
	FOR_SQ_IN_BB(atkr_sq, atkr_bb) { \
		if (!in_btwn_bb(atkr_sq, vctm_sq)) { \
			mv_t mv = MV_FROM_FTMCP(atkr_sq, vctm_sq, pc, vctm_pc, 0); \
			if (mv_is_good(mv)) \
				MV_SLCT_FN(mv); \
		} \
	}

/* Not a function, this is embedded inside another function with the #include. */
{
	int i;

	clr_t stm;
	clr_t sntm;

	bb_t vctm_bb;
	pc_t vctm_pc;
	sq_t vctm_sq;

	bb_t atkr_bb;
	pc_t atkr_pc;
	sq_t atkr_sq;
	bb_t atks[PC_COUNT];

	/* Try hash move. */
	if (mv_is_pseudo(slct->hash_mv))
		MV_SLCT_FN(slct->hash_mv);

	stm = board.stm;
	sntm = FLIP_COLOR(board.stm);

	/* Loop through victims in reverse order. This should be unrolled! */
	for (vctm_pc = QUEEN; vctm_pc >= PAWN; vctm_pc--) {
		vctm_bb = board.piece_bb[sntm][vctm_pc];

		/* Find pawn captures first. We can do this setwise, and we don't
		 * need SEE. */
		atkr_bb = board.piece_bb[stm][PAWN] & atks[PAWN];

		/* Loop through all pawn attacks, one direction at a time. */
		FOR_SQ_IN_BB(vctm_sq, CSH_DL(stm, vctm_bb) & atkr_bb) {
			atkr_sq = sq_ur[stm][vctm_sq];
			MV_SLCT_FN(MV_FROM_FT(atkr_sq, vctm_sq));
		}
		FOR_SQ_IN_BB(vctm_sq, CSH_DR(stm, vctm_bb) & atkr_bb) {
			atkr_sq = sq_ul[stm][vctm_sq];
			MV_SLCT_FN(MV_FROM_FT(atkr_sq, vctm_sq));
		}

		/* Find all the squares that the victim piece type is on. */
		FOR_SQ_IN_BB(vctm_sq, vctm_bb) {
			/* eh, waste of time, whatever. */
			atks[PAWN] = atk_bb_pre[vctm_sq][ATK_PRE_P(stm)];

			/* Knight attacks. */
			atks[KNIGHT] = atk_bb_pre[vctm_sq][ATK_PRE_N];
			atkr_bb = board.piece_bb[stm][KNIGHT] & atks[KNIGHT];

			CAP_LOOP(KNIGHT);

			/* Bishop attacks by direction. */
			bb_t b_atk = atk_bb_pre[vctm_sq][ATK_PRE_B];

			atkr_bb = b_atk & board.piece_bb[stm][BISHOP];
			SLDR_CAP_LOOP(BISHOP);

			/* Rook attacks by direction. */
			bb_t r_atk = atk_bb_pre[vctm_sq][ATK_PRE_R];

			atkr_bb = r_atk & board.piece_bb[stm][ROOK];
			SLDR_CAP_LOOP(ROOK);

			/* Queen attacks by direction. */
			bb_t q_atk = b_atk | r_atk;

			atkr_bb = q_atk & board.piece_bb[stm][QUEEN];
			SLDR_CAP_LOOP(QUEEN);

			/* King attacks. */
			if (vctm_pc != KING) {
				atks[KING] = atk_bb_pre[vctm_sq][ATK_PRE_K];
				atkr_bb = board.piece_bb[stm][KING] & atks[KING];

				CAP_LOOP(KING);
			}
		}
	}

	/* Non-quiescence part: killers, bad captures, and noncaptures */
#ifndef IS_QUIESCE
	/* Try killer moves. */
	for (i = 0; i < KILLER_COUNT; i++) {
		if (mv_is_pseudo(slct->kllr_mv[i]))
			MV_SLCT_FN(slct->kllr_mv[i]);
	}
	/* Try bad captures. */

	/* Generate attacks for noncaptures. */
	atk_set_gen(atks, stm);

	/* Try non captures in basically random order. */
#endif
}
