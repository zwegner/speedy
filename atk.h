/* atk.h */

#ifndef _ATK_H_
#define _ATK_H_

#include "speedy.h"
#include "chess.h"
#include "bb.h"

/* Types */
typedef struct
{
	bb_t atks[64];
	bb_t caps[16];
} atk_set_t;

#define ATK_PRE_P(c)	(c)
#define ATK_PRE_N		(2)
#define ATK_PRE_B		(3)
#define ATK_PRE_R		(4)
#define ATK_PRE_K		(5)

#define ATK_PRE_CT		(6)

/* Macros */

#define MASK_SHL (0x7f7f7f7f7f7f7f7full)
#define MASK_SHR (0xfefefefefefefefeull)

#define SH_UL(b)				(((b) << 7) & MASK_SHL)
#define SH_UP(b)				((b) << 8)
#define SH_UR(b)				(((b) << 9) & MASK_SHR)
#define SH_RT(b)				(((b) << 1) & MASK_SHR)
#define SH_DR(b)				(((b) >> 7) & MASK_SHR)
#define SH_DN(b)				((b) >> 8)
#define SH_DL(b)				(((b) >> 9) & MASK_SHL)
#define SH_LF(b)				(((b) >> 1) & MASK_SHL)

#define CSH_UL(c, bb) (((c)==CLR_W ? (bb<<7) : (bb>>9)) & MASK_SHL)
#define CSH_UR(c, bb) (((c)==CLR_W ? (bb<<9) : (bb>>7)) & MASK_SHR)
#define CSH_DL(c, bb) (((c)==CLR_W ? (bb>>9) : (bb<<7)) & MASK_SHL)
#define CSH_DR(c, bb) (((c)==CLR_W ? (bb>>7) : (bb<<9)) & MASK_SHR)

/* Prototypes */
bb_t in_btwn_bb(sq_t from, sq_t to);

/* Data */
extern sq_t sq_ul[CLR_CT][SQ_CT];
extern sq_t sq_ur[CLR_CT][SQ_CT];

extern bb_t atk_pre_bb[SQ_CT][ATK_PRE_CT];

extern bb_t in_btwn_pre_bb[SQ_CT][SQ_CT];

#define mv_is_good(x) 1

#endif
