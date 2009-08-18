CFILES=gen.c make.c see.c srch.c

HFILES=atk.h bb.h brd.h clr.h kogge.h mv.h mv_slct.h speedy.h

speedy: ${CFILES} ${HFILES}
	cc ${CFILES} -o speedy
