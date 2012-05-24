#ifndef LLO_INTERNAL_MOVES_CPP_
#define LLO_INTERNAL_MOVES_CPP_

#include "include/llo_internal.h"

inline 
void move_init(board_ptr brd, PIN pin, int &row, int &col, int &min_distance){
	row = pin / brd->board_width;
	col = pin % brd->board_width;
	min_distance = brd->board_width;
}

int move_up(board_ptr brd, int i, PIN out[]){
	int row, col, min_distance;
	move_init(brd, i, row, col, min_distance);

	int ii; // against pinIndex
	for(ii = 0; ii<brd->ctn_pins; ii++){
		if( ii == i ) continue; // same pin
		if( col != brd->pins[ii] % brd->board_width ) continue; // not on the same column
		int other_row = brd->pins[ii] / brd->board_width;
		if( other_row > row ) continue;	// behind another pin on the same column
		min_distance = MIN(row - other_row - 1, min_distance);
	}
	if( min_distance != brd->board_width && min_distance > 0){
		memcpy(out, brd->pins, sizeof(PIN) * brd->ctn_pins);
		out[i] = brd->pins[i] - min_distance * brd->board_width;
		return LLO_OK;
	}
	return LLO_ERROR;
}

int move_down(board_ptr brd, int i, PIN out[]){
	int row, col, min_distance;
	move_init(brd, i, row, col, min_distance);

	int ii; // against pinIndex
	for(ii = 0; ii<brd->ctn_pins; ii++){
		if( ii == i ) continue; // same pin
		if( col != brd->pins[ii] % brd->board_width ) continue; // not on the same column
		int other_row = brd->pins[ii] / brd->board_width;
		if( other_row < row ) continue;	// behind another pin on the same column
		min_distance = MIN(row - other_row - 1, min_distance);
	}
	if( min_distance != brd->board_width && min_distance > 0){
		memcpy(out, brd->pins, sizeof(PIN) * brd->ctn_pins);
		out[i] = brd->pins[i] + min_distance * brd->board_width;
		return LLO_OK;
	}
	return LLO_ERROR;
}

int move_left(board_ptr brd, int i, PIN out[]){
	int row, col, min_distance;
	move_init(brd, i, row, col, min_distance);

	int ii; // against pinIndex
	for(ii = 0; ii<brd->ctn_pins; ii++){
		if( ii == i ) continue; // same pin
		
		if( row != brd->pins[ii] / brd->board_width ) continue;	// behind another pin on the same column

		int other_col = brd->pins[ii] % brd->board_width;
		if( other_col > col ) continue; // not on the same column

		min_distance = MIN(col - other_col - 1, min_distance);
	}
	if( min_distance != brd->board_width && min_distance > 0){
		memcpy(out, brd->pins, sizeof(PIN) * brd->ctn_pins);
		out[i] = brd->pins[i] - min_distance;
		return LLO_OK;
	}
	return LLO_ERROR;
}

int move_right(board_ptr brd, int i, PIN out[]){
	int row, col, min_distance;
	move_init(brd, i, row, col, min_distance);

	int ii; // against pinIndex
	for(ii = 0; ii<brd->ctn_pins; ii++){
		if( ii == i ) continue; // same pin
		if( row != brd->pins[ii] / brd->board_width ) continue;	// behind another pin on the same column

		int other_col = brd->pins[ii] % brd->board_width;
		if( other_col < col ) continue; // not on the same column
		min_distance = MIN(col - other_col - 1, min_distance);
	}
	if( min_distance != brd->board_width && min_distance > 0){
		memcpy(out, brd->pins, sizeof(PIN) * brd->ctn_pins);
		out[i] = brd->pins[i] + min_distance;
		return LLO_OK;
	}
	return LLO_ERROR;
}

#endif // LLO_INTERNAL_MOVES_CPP_