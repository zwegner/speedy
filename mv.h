/* mv.h */

#include "speedy.h"
#include "bb.h"

typedef unsigned int mv_t;
typedef unsigned char cstl_t;

#define MV_FROM_FTP(f,t,p) (p<<12|f<<6|t)

struct undo_t
{
	mv_t mv;
	bb_t ep;
	cstl_t cstl;
};
