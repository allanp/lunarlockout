#ifndef __LLO_DFS_H__
#define __LLO_DFS_H__

#include "llo_defs.h"

int solve_dfs(PIN* board, PIN* result, PIN movements[][2], int& moves, const int& start_index){
 
  // some initialization setup
  PIN boards[MAX_MOVES+1][NUM_PINS+2]; // {P[0]...P[NUM_PINS-1] + PinIdx + Direction}
 
  PIN *p, *out;
 
  int pp = 0;   // pin index [0 ~ NUM_PINS-1]
  int dd = UP;  // UP = 0(^), RIGHT = 1(>), DOWN = 2(v), LEFT = 3(<)
  int moved = LLO_FAILED;
  size_t size_of_board = sizeof(PIN) * (NUM_PINS+2);
 
  out = (PIN*)malloc(size_of_board);
  memset(boards, (PIN)-1, size_of_board * MAX_MOVES );
  int size = -1;
  int max_step = moves;
  int solution = 0;
 
  int mark_p, mark_d;

  // push board into stack
  S_PUSH(boards, board, size, size_of_board);
 
  mark_p = board[iPIN];
  mark_d = board[iDIR];

  VISIT(boards[0], 0, UP);

  int count = 0;
 
  while( size >= 0 ){
    // step back until less than max_step
    if( size == max_step ) {
      size--; boards[size][iDIR]++;
      if (size>0) {
        mark_p = boards[size-1][iPIN]; mark_d = boards[size-1][iDIR];
      }
      else {
        mark_p = 1; mark_d = 1;
      }
    }

    // peek the stack
    p = S_PEEK(boards, size);  // get the board

    // try access an ajacent node of p
    int moved;
    if (p[iDIR] == 4)
      moved = LLO_FAILED;
    // ignore same pp and opposite dd
    else if( p[iPIN] == mark_p && p[iDIR] == rid_int[mark_d]) 
      moved = LLO_FAILED;
    else
      moved = (p[iDIR] < 4) ? move_fp[p[iDIR]](p,  p[iPIN], out) : LLO_FAILED;

    if( moved == LLO_SUCCEED){
      mark_p = p[iPIN];
      mark_d = p[iDIR];

      if(out[0] == BOARD_CENTER){       
        out[iPIN] = p[iPIN]; out[iDIR] = 4;
        solution++;
        max_step = size + 1;
        memcpy(result, out, sizeof(PIN) * (NUM_PINS + 2) );
        moves = max_step;
        int m = 0;
        for(m = 0; m < max_step; m++){
          movements[m][0] = boards[m][iPIN]; movements[m][1] = boards[m][iDIR];
        }
      }

      // if not solution, push 'out' into the boards, and setup starting pp and direction
      S_PUSH(boards, out, size, size_of_board);
      VISIT(boards[size], 0, UP);
    }
    else if( moved == LLO_FAILED){
      p[iDIR]++;
      if(p[iDIR] >= 4){
        p[iDIR] = 0;
        p[iPIN]++;
        if(p[iPIN] >= NUM_PINS)
          p[iPIN] = 0;
      }
      
      if( size >= 0 && boards[size][iDIR] == UP && boards[size][iPIN] == 0 ){
        if( size >= 0 )
          S_POP(boards, size); // step back until there is avaiable moves left for the current board

        if(size >= 0 ) {
          boards[size][iDIR]++;    // next move,
          if (size > 0) {
            mark_p = boards[size-1][iPIN]; 
            mark_d = boards[size-1][iDIR];
          }
          else {
            mark_p=1; mark_d=1;
          }
        }
      }
    }
    if(solution > 0)
      break;
  }
  return solution > 0 ? LLO_SUCCEED : LLO_FAILED;
}

#endif // __LLO_DFS_H__
