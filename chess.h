/* chess.h */

#ifndef _CHESS_H_
#define _CHESS_H_

#include "speedy.h"
#include "bb.h"

/* Colors */

typedef s32_t clr_t;
enum { CLR_W, CLR_B };

#define CLR_CT (2)

#define FLP_CLR(c)	((c)^1)
#define FOR_EACH_CLR(c) for (c = CLR_W; c <= CLR_B; c++)

/* Pieces */

typedef s32_t pc_t;
enum { PC_P, PC_N, PC_B, PC_R, PC_Q, PC_K };

#define PC_CT (6)

#define FOR_EACH_PC(p) for (p = PC_P; p <= PC_K; p++)

/* Squares */

typedef s32_t sq_t;

#define SQ_CT (64)

#define FOR_EACH_SQ(s) for (s = 0; s <= 63; s++)

#define SQ_IS_PROM(s) ((unsigned)(s)-8 >= 060)

/* Square printing: hacky, but easier than a function */
#define SQ_FMT  "%c%i"
#define SQ_ARGS(sq)  ((sq)&7)+'a',((sq)>>3)+1

/* Moves */

typedef u32_t mv_t;

/*
 * Move structure:
 * 0000 0000 0000 0000 0011 1111 to
 * 0000 0000 0000 1111 1100 0000 from
 * 0000 0000 0111 0000 0000 0000 moving pc
 * 0000 0011 1000 0000 0000 0000 cap pc
 * 0001 1100 0000 0000 0000 0000 promote pc
 *
 * total=21 bits.
 */
#define MV_FROM_FTMCP(f,t,m,c,p)	(t|f<<6|m<<12|c<<15|p<<18)
#define MV_GET_T(mv)				(mv&077)
#define MV_GET_F(mv)				(mv>>6&077)
#define MV_GET_M(mv)				(mv>>12&7)
#define MV_GET_C(mv)				(mv>>15&7)
#define MV_GET_P(mv)				(mv>>18)
#define MV_SET_P(prom)				(prom<<18)

/* Castling Rights */

typedef u8_t cstl_t;

#define CAN_CSTL(c,s) ((c)>>(s)&5)
#define CAN_CSTL_KS(c,s) ((c)>>(s)&1)
#define CAN_CSTL_QS(c,s) ((c)>>(s)&4)

/* Magic castle rights updating */
#define CSTL_MSK (0x9100000000000091)
#define CSTL_MGC (0x0410000000000021)

/* Board */

typedef struct
{
	bb_t pc_bb[6];
	bb_t clr_bb[2];
	clr_t stm;
	bb_t ep;
	cstl_t cstl;
} brd_t;

#define BRD_GET_PC(b,p,c) ((b)->pc_bb[p] & (b)->clr_bb[c])

/* Prototypes */


/* Data */
extern cstl_t cstl_tbl[64]; /* Magic castling */

#endif
