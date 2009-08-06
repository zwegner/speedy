// More stuff ripped off from ZCT!!
#define FILL_FUNC_SH(inc, SH)							\
	do {												\
		piece |= empty & (piece SH inc);				\
		empty &= (empty SH inc);						\
		piece |= empty & (piece SH (inc * 2));			\
		empty &= (empty SH (inc * 2));					\
		return piece | (empty & (piece SH (inc * 4)));	\
	} while (0)
#define FILL_ATTACKS_FUNC(dir, inc, SH) \
    return kogge_stone_fill_##dir(piece, empty) SH inc \

// Generic filler: We use a macro to create each function. The compiler will
//	optimize away all of the conditionals as they are based on constants.
#define FILL_FUNC(dir, inc, sh)								\
	static bitboard kogge_stone_fill_##dir(bitboard piece, bitboard empty)	\
	{													\
		const int shift = abs(inc);						\
		/* Weird--find the rank offset of inc.*/		\
		if ((inc + 16) % 8 == 1)						\
			empty &= p_leftmask;							\
		else if ((inc + 16) % 8 == 7)					\
			empty &= p_rightmask;							\
		if (inc > 0)									\
			FILL_FUNC_SH(shift, <<);					\
		else											\
			FILL_FUNC_SH(shift, >>);					\
	} \
  static zforceinline bitboard fill_attacks_##dir(bitboard piece, bitboard empty) \
  { \
    return SHIFT_##sh(kogge_stone_fill_##dir(piece, empty));\
  }

FILL_FUNC(up_right, 9, UR)
FILL_FUNC(up, 8, UP)
FILL_FUNC(up_left, 7, UL)
FILL_FUNC(right, 1, RT)
FILL_FUNC(down_right, -7, DR)
FILL_FUNC(down, -8, DN)
FILL_FUNC(down_left, -9, DL)
FILL_FUNC(left, -1, LF)
