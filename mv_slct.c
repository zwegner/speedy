/* mv_slct.c */

/* This file works as a move selector, but to make the code cleaner/faster
 * at the expense of some code size, we implement it as a seperate file
 * with a function defined as a macro before each inclusion. This generates
 * moves as a finite state machine. It basically works like Ken Thompson's
 * Belle in bitboards, but more complex. We don't want to deal with
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
#	define ADD_BAD_CAP(mv) slct->bad_caps[slct->bad_cap_ct++] = mv
#endif

#ifdef PREFIX
#define _LBL(p,x) p##x
#define LBL(p,x) _LBL(p,x)
#else
#define _LBL(p,x) p##x##x /* Bogus */
#define LBL(p,x) _LBL(p,x)
#endif

#define CAP_LOOP(pc) \
	FOR_SQ_IN_BB(atkr_sq, atkr_bb) { \
		mv_t mv_ = MV_FROM_FTMCP(atkr_sq, vctm_sq, pc, vctm_pc, 0); \
		LBL(PREFIX,__LINE__):\
		if (mv_ != slct->hash_mv) { \
			if (mv_is_good(mv_)) \
				do { MV_SLCT_FN(mv_); } while (0); \
			else \
				ADD_BAD_CAP(mv_); \
		} \
	}

#define SLDR_CAP_LOOP(pc) \
	FOR_SQ_IN_BB(atkr_sq, atkr_bb) { \
		if (!(in_btwn_bb(atkr_sq, vctm_sq) & occ)) { \
			mv_t mv_ = MV_FROM_FTMCP(atkr_sq, vctm_sq, pc, vctm_pc, 0); \
			LBL(PREFIX,__LINE__):\
			if (mv_ != slct->hash_mv) { \
				if (mv_is_good(mv_)) \
				do { MV_SLCT_FN(mv_); } while (0); \
				else \
					ADD_BAD_CAP(mv_); \
			} \
		} \
	}

#define MV_IS_DUP(mv) (mv != slct->hash_mv & \
		mv != slct->kllr_mv[0] & \
		mv != slct->kllr_mv[1])

/* In quiescence, we have a minimum piece value for captures in order to be
 * not futile. It defaults to pawn, but can be set to something higher based
 * on the evaluation and alpha. */
#ifndef MIN_VCTM
#define MIN_VCTM PC_P
#endif

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

	bb_t mv_bb;

	bb_t occ = brd->clr_bb[0] | brd->clr_bb[1];

	mv_t mv;
	mv_t mv_2;

	/* Try hash move. */
	mv = slct->hash_mv;
	if (mv_is_pseudo(mv))
		MV_SLCT_FN(mv);

#define CHECK_DUP(m)	((m) != slct->hash_mv)
	stm = brd->stm;
	sntm = FLP_CLR(brd->stm);

	/* Compute some initial info. */
	atkr_bb = BRD_GET_PC(brd, PC_P, stm);

	/* Loop through victims in reverse order. This should be unrolled! */
	for (vctm_pc = PC_Q; vctm_pc >= MIN_VCTM; vctm_pc--) {
		vctm_bb = BRD_GET_PC(brd, vctm_pc, sntm);

		/* Find pawn captures first. We can do this setwise, and we don't
		 * need SEE. */
		atkr_bb = CSH_DL(stm, vctm_bb) & BRD_GET_PC(brd, PC_P, stm);

		/* Loop through all pawn attacks, one direction at a time. */
		FOR_SQ_IN_BB(atkr_sq, atkr_bb) {
			vctm_sq = sq_ur[stm][atkr_sq];
			mv = MV_FROM_FTMCP(atkr_sq, vctm_sq, PC_P, vctm_pc, 0);
			if (SQ_IS_PROM(vctm_sq)) {
				/* Add underpromotions as "bad captures". */
				ADD_BAD_CAP(mv | MV_SET_P(PC_N));
				ADD_BAD_CAP(mv | MV_SET_P(PC_R));
				ADD_BAD_CAP(mv | MV_SET_P(PC_B));
				mv |= MV_SET_P(PC_Q);
			}
			if (mv != slct->hash_mv)
			if (CHECK_DUP(mv))
				MV_SLCT_FN(mv);
		}

		atkr_bb = CSH_DR(stm, vctm_bb) & BRD_GET_PC(brd, PC_P, stm);

		FOR_SQ_IN_BB(atkr_sq, atkr_bb) {
			vctm_sq = sq_ul[stm][atkr_sq];
			mv = MV_FROM_FTMCP(atkr_sq, vctm_sq, PC_P, vctm_pc, 0);
			if (SQ_IS_PROM(vctm_sq)) {
				/* Add underpromotions as "bad captures". */
				ADD_BAD_CAP(mv | MV_SET_P(PC_N));
				ADD_BAD_CAP(mv | MV_SET_P(PC_R));
				ADD_BAD_CAP(mv | MV_SET_P(PC_B));
				mv |= MV_SET_P(PC_Q);
			}
			if (CHECK_DUP(mv))
				MV_SLCT_FN(mv);
		}

		/* Find all the squares that the victim piece type is on. */
		FOR_SQ_IN_BB(vctm_sq, vctm_bb) {
			/* eh, waste of time, whatever. */

			/* Knight attacks. */
			bb_t atkr_n = atk_pre_bb[vctm_sq][ATK_PRE_N];
			atkr_bb = BRD_GET_PC(brd, PC_N, stm) & atkr_n;

			CAP_LOOP(PC_N);

			/* Bishop attacks by direction. */
			bb_t b_atk = atk_pre_bb[vctm_sq][ATK_PRE_B];

			atkr_bb = b_atk & BRD_GET_PC(brd, PC_B, stm);
			SLDR_CAP_LOOP(PC_B);

			/* Rook attacks by direction. */
			bb_t r_atk = atk_pre_bb[vctm_sq][ATK_PRE_R];

			atkr_bb = r_atk & BRD_GET_PC(brd, PC_R, stm);
			SLDR_CAP_LOOP(PC_R);

			/* Queen attacks by direction. */
			bb_t q_atk = b_atk | r_atk;

			atkr_bb = q_atk & BRD_GET_PC(brd, PC_Q, stm);
			SLDR_CAP_LOOP(PC_Q);

			/* King attacks. */
			if (vctm_pc != PC_K) {
				bb_t atkr_k = atk_pre_bb[vctm_sq][ATK_PRE_K];
				atkr_bb = BRD_GET_PC(brd, PC_K, stm) & atkr_k;

				CAP_LOOP(PC_K);
			}
		}
	}

	/* Non-quiescence part: killers, bad captures, and noncaptures */
#ifdef IS_QUIESCE

	/* Try killer moves. */
	for (i = 0; i < KILLER_CT; i++) {
		mv = slct->kllr_mv[i];
		if (mv != slct->hash_mv & mv_is_pseudo(mv))
			MV_SLCT_FN(mv);
	}

#undef CHECK_DUP
#define CHECK_DUP(m)	((m) != slct->hash_mv & \
						(m) != slct->kllr_mv[0] & \
						(m) != slct->kllr_mv[1])

	/* Try bad captures. */
	for (i = 0; i < slct->bad_cap_ct; i++) {
		mv = slct->bad_cap[i];
		/* Check for duplicate moves. */
		MV_SLCT_FN(mv);
	}

	/* Generate attacks for noncaptures. */

	/* Promotions */
	atkr_bb = CSH_DN(stm, ~occ) & BRD_GET_PC(brd, PC_P, stm) &
		C_RNK_MSK(stm, RNK_7);

	FOR_SQ_IN_BB(atkr_sq, atkr_bb) {
		mv = MV_FROM_FTMCP(atkr_sq, sq_up[stm][atkr_sq], PC_P, 0, 0);
		if (SQ_IS_PROM(sq_up[stm][atkr_sq])) {
			for (pc = PC_Q; pc >= PC_N; pc--) {
				mv_2 = mv | MV_SET_P(pc);

	/* Castle moves */
	if (CAN_CSTL(brd->cstl, stm)) {
	}

	/* Try pawn non-caps first. */

	atkr_bb = CSH_DN(stm, ~occ) & BRD_GET_PC(brd, PC_P, stm) &
		~C_RNK_MSK(stm, RNK_7);

	FOR_SQ_IN_BB(atkr_sq, atkr_bb) {
		mv = MV_FROM_FTMCP(atkr_sq, sq_up[stm][atkr_sq], PC_P, 0, 0);
		if (SQ_IS_PROM(sq_up[stm][atkr_sq])) {
			for (pc = PC_Q; pc >= PC_N; pc--) {
				mv_2 = mv | MV_SET_P(pc);

				if (CHECK_DUP(mv_2))
					MV_SLCT_FN(mv_2);
			}
		} else if (CHECK_DUP(mv_2))
			MV_SLCT_FN(mv);
	}


	/* Try non captures in basically random order. */
	/* XXX priority bitscan? */

#endif
}

#undef ADD_BAD_CAP
#undef MIN_VCTM
#undef MV_IS_DUP
#undef SLDR_CAP_LOOP
#undef CAP_LOOP
#undef _LBL
#undef LBL

#if 0
void initialize_attacks(void)
{
	BITBOARD mask;
	BITBOARD submask;
	SQUARE square;
	int diag;

	for (square = 0; square < OFF_BOARD; square++)
	{
		/* Calculate the attack mask for ranks. */
		attack_mask[square][DIR_HORI] = MASK_RANK(RANK_OF(square)) &
			~(MASK_FILE(FILE_A) | MASK_FILE(FILE_H));

		/* Calculate the attack mask for A1H8 diagonals. */
		diag = FILE_OF(square) - RANK_OF(square);
		if (diag > 0)
			attack_mask[square][DIR_A1H8] =
				0x0040201008040200ull >> (diag * 8);
		else
			attack_mask[square][DIR_A1H8] =
				0x0040201008040200ull << (-diag * 8);
		/* Mask off the outer bits. */
		attack_mask[square][DIR_A1H8] &=
			~(MASK_RANK(RANK_1) | MASK_RANK(RANK_8));

		/* Calculate the attack mask for A8H1 diagonals. */
		diag = FILE_OF(square) + RANK_OF(square) - 7;
		if (diag > 0)
			attack_mask[square][DIR_A8H1] =
				0x0002040810204000ull << (diag * 8);
		else
			attack_mask[square][DIR_A8H1] =
				0x0002040810204000ull >> (-diag * 8);
		/* Mask off the outer bits. */
		attack_mask[square][DIR_A8H1] &=
			~(MASK_RANK(RANK_1) | MASK_RANK(RANK_8));

		/* Calculate the attack mask for files. */
		attack_mask[square][DIR_VERT] = MASK_FILE(FILE_OF(square)) &
			~(MASK_RANK(RANK_1) | MASK_RANK(RANK_8));
	}

	/* For each direction, loop over all possible bit-states for the occupied
		set, produce an attack board using the Kogge Stone fill routines, and
		store it in the attack arrays. This uses Steffen Westcott's subset
		traversing algorithm. */
	for (square = 0; square < OFF_BOARD; square++)
	{
		/* Rank attacks. */
		mask = attack_mask[square][DIR_HORI];
		submask = 0;
		do
		{
			submask = (submask - mask) & mask;
			attack_bb[square][DIR_HORI][attack_index(submask, DIR_HORI)] =
				SHIFT_RT(fill_right(MASK(square), ~submask)) |
				SHIFT_LF(fill_left(MASK(square), ~submask));
		} while (submask);
		/* A8H1 attacks. */
		mask = attack_mask[square][DIR_A1H8];
		submask = 0;
		do
		{
			submask = (submask - mask) & mask;
			attack_bb[square][DIR_A1H8][attack_index(submask, DIR_A1H8)] =
				SHIFT_DL(fill_down_left(MASK(square), ~submask)) |
				SHIFT_UR(fill_up_right(MASK(square), ~submask));
		} while (submask);
		/* ADIR_A1H8 attacks. */
		mask = attack_mask[square][DIR_A8H1];
		submask = 0;
		do
		{
			submask = (submask - mask) & mask;
			attack_bb[square][DIR_A8H1][attack_index(submask, DIR_A8H1)] =
				SHIFT_DR(fill_down_right(MASK(square), ~submask)) |
				SHIFT_UL(fill_up_left(MASK(square), ~submask));
		} while (submask);
		/* File attacks. */
		mask = attack_mask[square][DIR_VERT];
		submask = 0;
		do
		{
			submask = (submask - mask) & mask;
			attack_bb[square][DIR_VERT][attack_index(submask, DIR_VERT)] =
				SHIFT_UP(fill_up(MASK(square), ~submask)) |
				SHIFT_DN(fill_down(MASK(square), ~submask));
		} while (submask);
	}
}
#endif
