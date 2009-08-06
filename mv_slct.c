/* move_selector.c */

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
 * MV_SLCT_FN_NAME: the name to call the function that is generated
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
 * Killer moves 1/2 (if not quiescent)
 * Captures in MVV/LVA order, after testing with SEE
 * Noncaptures
 * Bad captures (found during the MVV/LVA phase, tested in that order)
 *
 */

#ifdef IS_QUIESCE
#	define ADD_BAD_CAP(mv)
#else
#	define ADD_BAD_CAP(mv) slct->bad_caps[slct->bad_cap_count++] = mv
#endif

/* Not a function, this is embedded inside another function with the #include. */
{
	int i;

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

#ifndef IS_QUIESCE
	/* Try killer moves. */
	for (i = 0; i < KILLER_COUNT; i++) {
		if (mv_is_pseudo(slct->kllr_mv[i]))
			MV_SLCT_FN(slct->kllr_mv[i]);
	}
#endif

	/* Loop through victims in reverse order. This should be unrolled! */
	for (vctm_pc = QUEEN; vctm_pc >= PAWN; vctm_pc--) {
		vctm_bb = board.piece_bb[FLIP_COLOR(board.stm)][vctm_pc];

		/* Find pawn captures first. We can do this setwise, and we don't
		 * need SEE. */
		atkr_bb = board.piece_bb[board.stm][PAWN] & atks[PAWN];

		/* Loop through all pawn attacks, one direction at a time. */
		FOR_SQ_IN_BB(vctm_sq, CSH_DL(board.stm, vctm_bb) & atkr_bb) {
			atkr_sq = sq_ur[board.stm][vctm_sq];
			MV_SLCT_FN(MV_FROM_FT(atkr_sq, vctm_sq));
		}
		FOR_SQ_IN_BB(vctm_sq, CSH_DR(board.stm, vctm_bb) & atkr_bb) {
			atkr_sq = sq_ul[board.stm][vctm_sq];
			MV_SLCT_FN(MV_FROM_FT(atkr_sq, vctm_sq));
		}

		/* Find all the squares that the victim piece type is on. */
		FOR_SQ_IN_BB(vctm_sq, vctm_bb) {
			/* eh, waste of time, whatever. */
			atks[PAWN] = atk_bb_pre[vctm_sq][ATK_PRE_P(board.stm)];

			/* Knight attacks. */
			atks[KNIGHT] = atk_bb_pre[vctm_sq][ATK_PRE_N];
			atkr_bb = board.piece_bb[board.stm][KNIGHT] & atks[KNIGHT];

			FOR_SQ_IN_BB(atkr_sq, atkr_bb)
				MV_SLCT_FN(MV_FROM_FT(atkr_sq, vctm_sq));

			/* Bishop attacks by direction. */

			/* Rook attacks by direction. */

			/* Queen attacks by direction. */

			/* King attacks. */
			atks[KING] = atk_bb_pre[vctm_sq][ATK_PRE_K];
			atkr_bb = board.piece_bb[board.stm][KING] & atks[KING];

			FOR_SQ_IN_BB(atkr_sq, atkr_bb)
				MV_SLCT_FN(MV_FROM_FT(atkr_sq, vctm_sq));
		}
	}

	/* Non-quiescence part: bad captures and noncaptures */
#ifndef IS_QUIESCE
	/* Try bad captures. */

	/* Generate attacks for noncaptures. */
	atk_set_gen(atks, board.stm);

	/* Try non captures in basically random order. */
#endif
}
