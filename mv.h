/* mv.h */

#ifndef _MV_H_
#define _MV_H_

#include "speedy.h"
#include "bb.h"

typedef unsigned int mv_t;
typedef unsigned char cstl_t;

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

#endif
