int castle_magic_idx(bb_t mask)
{
	return mask * castle_magic >> 58;
}

void make(int pc_move, int pc_cap, bb_t from, bb_t to)
{

	/* Remove captured piece. */
	board.piece_bb[pc_cap] &= ~to;
	board.occ_bb[FLIP_SIDE(board.side_tm)] &= ~to;

	/* Set moving piece. */
	board.piece_bb[pc_move] &= ~from;
	board.occ_bb[board.side_tm] &= ~from;
	board.piece_bb[pc_move] |= to;
	board.occ_bb[board.side_tm] |= to;

	board.side_tm = FLIP_SIDE(board.side_tm);

	/* Update castling rights. */
	mask = (from | to) & castle_rights_mask;
	board.castle &= castle_table[castle_magic_idx(mask)];
}
