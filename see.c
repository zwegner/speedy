/* see.c */

bool_t popcnt_gt(bb_t a, bb_t b)
{
	while (a!=0 & b!=0) {
		a &= a - 1;
		b &= b - 1;
	}
	return a!=0;
}

enum see_pc { SEE_P, SEE_BN, SEE_R, SEE_Q, SEE_K };

/*
 * NxP
 *
 * val=1, pc=3
 * def= nb q
 * atk=   rq
 *
 * pop(def_nb)>pop(atk_nb)
 *
 * val=-2, pc=3
 * def=  b q
 * atk=   rq
 *
 * pop(def_nb)>pop(atk_nb)
 *
 * stand pat, val=-2, cap=bad
 *
 * PxP
 *
 * val=1, pc=1
 * def= nb q
 * atk= n rq
 *
 * pop(def_nb)>pop(atk_nb)
 * swap
 *
 * val=1, pc=3
 * def=  b q
 * atk=   rq
 *
 * val=-2, pc=3
 * def=    q
 * atk=   rq
 *
 * val=1, pc=5
 * def=    q
 * atk=    q
 *
 * (def-atk)*q
 *
 */
bool_t mv_is_good(mv_t mv)
{
	int val;
	int pc_val;
	int see_val[5] = { 1, 3, 5, 9, 1000 };
	bb_t atk_bb[5];
	bb_t def_bb[5];

	atk_bb[pc] &= ~from;

	/* Get values after making first capture. */
	val = see_val[cap];
	pc_val = see_val[pc];

	/* Loop through the pieces, first with defender then attacker. If the
	 * defender can stand pat with the value < 0 or the attacker with >= 0, we
	 * can stop. */
	for (def_pc = PAWN; def_pc <= KING; def_pc++) {
defend:
		if (val < 0)
			return FALSE;

		defs = popcnt(def_bb[pc]);

		if (defs) {
		}
	}

	for (atk_pc = PAWN; atk_pc <= KING; atk_pc++) {
attack:

		atks = popcnt(atk_bb[pc]);

		if () {
		}
	}
}
