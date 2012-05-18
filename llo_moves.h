#ifndef __LLO_MOVES_H__
#define __LLO_MOVES_H__

#include "llo_defs.h"

/** move functions */
// input[]: input board
// i      : pin index
// out[]  : output board
// pp     : pin position at the input board

inline void move_init(PIN &pp, int &row, int &col, int &min_distance){
  row = pp / BOARD_WIDTH;
  col = pp % BOARD_WIDTH;
  min_distance = BOARD_WIDTH;
}

int move_up(PIN input[], int i, PIN out[]){
  int row, col, min_distance;
  move_init(input[i], row, col, min_distance);

  int ii; // against pinIndex
  for(ii = 0; ii<NUM_PINS; ii++){
    if( ii == i ) continue;
    int other_row = input[ii] / BOARD_WIDTH;
    int other_col = input[ii] % BOARD_WIDTH;
    if( other_col != col ) continue;
    if( other_row > row ) continue;
    min_distance = MIN(row - other_row - 1, min_distance);
  }
  if( min_distance != BOARD_WIDTH && min_distance > 0){
    memcpy(out, input, board_size_in_bytes);
    out[i] = input[i] - min_distance * BOARD_WIDTH;
    return LLO_SUCCEED;
  }
  return LLO_FAILED;
}

int move_down(PIN input[], int i, PIN out[]){
  int row, col, min_distance;
  move_init(input[i], row, col, min_distance);

  int ii; // against pinIndex
  for(ii = 0; ii<NUM_PINS; ii++){
    if( ii == i ) continue;
    int other_row = input[ii] / BOARD_WIDTH;
    int other_col = input[ii] % BOARD_WIDTH;
    if( other_col != col ) continue;
    if( other_row < row ) continue;
    min_distance = MIN(other_row - row - 1, min_distance);
  }
  if( min_distance != BOARD_WIDTH && min_distance > 0){
    memcpy(out, input, board_size_in_bytes);
    out[i] = input[i] + min_distance * BOARD_WIDTH;
    return LLO_SUCCEED;
  }
  return LLO_FAILED;
}

int move_left(PIN input[], int i, PIN out[]){
  int row, col, min_distance;
  move_init(input[i], row, col, min_distance);

  int ii; // against pinIndex
  for(ii = 0; ii<NUM_PINS; ii++){
    if( ii == i ) continue;
    int other_row = input[ii] / BOARD_WIDTH;
    int other_col = input[ii] % BOARD_WIDTH;
    if( other_row != row ) continue;
    if( other_col > col ) continue;
    min_distance = MIN(col - other_col - 1, min_distance);
  }
  if( min_distance != BOARD_WIDTH && min_distance > 0){
    memcpy(out, input, board_size_in_bytes);
    out[i] = input[i] - min_distance;
    return LLO_SUCCEED;
  }
  return LLO_FAILED;
}

int move_right(PIN input[], int i, PIN out[]){
  int row, col, min_distance;
  move_init(input[i], row, col, min_distance);

  int ii; // against pinIndex
  for(ii = 0; ii < NUM_PINS; ii++){
    if( ii == i ) continue;
    int other_row = input[ii] / BOARD_WIDTH;
    int other_col = input[ii] % BOARD_WIDTH;
    if( other_row != row ) continue;
    if( other_col < col ) continue;
    min_distance = MIN(other_col - col - 1, min_distance);
  }
  if( min_distance != BOARD_WIDTH && min_distance > 0){
    memcpy(out, input, board_size_in_bytes);
    out[i] = input[i] + min_distance;
    return LLO_SUCCEED;
  }
  return LLO_FAILED;
}

#endif // __LLO_MOVES_H__