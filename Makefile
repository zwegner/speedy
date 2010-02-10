CFILES=mv.c srch.c atk.c init.c main.c brd.c

HFILES=atk.h bb.h chess.h mv_slct.h speedy.h

speedy: ${CFILES} ${HFILES} mv_slct.c
	cc ${CFILES} -g -o speedy
