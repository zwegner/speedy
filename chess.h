/* chess.h */

#ifndef _CHESS_H_
#define _CHESS_H_

#include "speedy.h"
#include "bb.h"

/* Colors */

typedef s32_t clr_t;
enum { CLR_W, CLR_B };

/* Pieces */

typedef s32_t pc_t;
enum { PC_P, PC_N, PC_B, PC_R, PC_Q, PC_K };

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

/* Castling Rights */

typedef u8_t cstl_t;

#define CSTL_

/* Board */

typedef struct
{
	bb_t pc_bb[6];
	bb_t clr_bb[2];
	clr_t stm;
	bb_t ep;
	cstl_t cstl;
} brd_t;

/* Prototypes */


/* Data */


#endif
