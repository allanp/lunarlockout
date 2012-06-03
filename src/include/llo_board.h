#ifndef LLO_BOARD_H_
#define LLO_BOARD_H_

#include "llo_internal.h"

#define BOARD_SIZE_BYTES(ctn_pins)	(sizeof(PIN) * ctn_pins)
#define BOARD_CENTER(board_width)	((board_width * board_width) >> 1)

int board_init(board_ptr brd, int board_width, int ctn_pins);
int board_copy(board_ptr input, board_ptr output);
void board_destroy(board_ptr brd);
int board_clear(board_ptr brd);

int board_result_init(board_result_ptr result, int board_width, int ctn_pins, int max_steps);
int board_result_final_to_board(board_result_ptr result, board_ptr output);

int board_result_merge(board_result_ptr b0, board_result_ptr b1, board_result_ptr output);


int board_max_ctn(int ctn_pins, int max_bfs);

#endif // LLO_BOARD_H_