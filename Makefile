CFILES=mv.c see.c srch.c

HFILES=atk.h bb.h chess.h mv_slct.h speedy.h

speedy: ${CFILES} ${HFILES}
	cc ${CFILES} -o speedy
