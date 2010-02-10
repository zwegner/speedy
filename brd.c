/* brd.c */

#include <stdio.h>

#include "speedy.h"
#include "chess.h"

void parse_fen(brd_t *brd, char *fen)
{
	char *c;
	int rank, file;
	clr_t clr;
	pc_t pc;
	sq_t sq;

	FOR_EACH_PC(pc)
		brd->pc_bb[pc] = 0;
	FOR_EACH_CLR(clr)
		brd->clr_bb[clr] = 0;

	rank = 7; file = 0;
	for (c = fen; *c != '\0' && *c != ' '; c++) {
		sq = rank*8+file;
		switch (*c) {

#define CASE(l,c,p) case l: \
			brd->pc_bb[p] |= MASK(sq); \
			brd->clr_bb[c] |= MASK(sq); \
			file++; \
			break

			CASE('P', CLR_W, PC_P);
			CASE('N', CLR_W, PC_N);
			CASE('B', CLR_W, PC_B);
			CASE('R', CLR_W, PC_R);
			CASE('Q', CLR_W, PC_Q);
			CASE('K', CLR_W, PC_K);
			CASE('p', CLR_B, PC_P);
			CASE('n', CLR_B, PC_N);
			CASE('b', CLR_B, PC_B);
			CASE('r', CLR_B, PC_R);
			CASE('q', CLR_B, PC_Q);
			CASE('k', CLR_B, PC_K);

			case '1'...'9':
				file += *c - '0';
				break;
			case '/':
				rank--;
				file = 0;
				break;
		}
	}

	c++;
	brd->stm = *c == 'w' ? CLR_W : CLR_B;
}

void display_board(brd_t *brd)
{
	int rank, file;
	const char pc_char[] = "pnbrqk";
	bb_t mask;
	pc_t pc;
	sq_t sq;

	FOR_EACH_PC(pc) {
		printf("pc %i:\n",pc);
		print_bb(brd->pc_bb[pc]);
	}
	for (rank = 7; rank >= 0; rank--) {
		for (file = 0; file < 8; file++) {
			sq = rank*8+file;
			mask = MASK(sq);
			if (!(brd->clr_bb[CLR_W] & mask) && !(brd->clr_bb[CLR_B] & mask))
				printf("-");
			else {
				FOR_EACH_PC(pc) {
					if (brd->pc_bb[pc] & mask) {
						if (brd->clr_bb[CLR_W] & mask)
							printf("%c", pc_char[pc]+('A'-'a'));
						else
							printf("%c", pc_char[pc]);
					}
				}
			}
			printf(" ");
		}
		printf("\n");
	}
}
