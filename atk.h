#include "chess.h"

/* Attack set */
struct attack_set_t
{
	bb_t attacks[64];
	bb_t captures[16];
};
