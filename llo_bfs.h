#ifndef __LLO_BFS_H__
#define __LLO_BFS_H__

#include "llo_defs.h"

int solve_bfs(PIN* root, int& max_level, PIN queue[][NUM_PINS+2], int* parents, int& start_index, int& num_result){

  if(root[0] == BOARD_CENTER){
    start_index = 0;
    num_result = 1;    
    return LLO_SUCCEED;
  }

  static size_t size_of_board = sizeof(PIN) * (NUM_PINS + 2);

  PIN *out = (PIN*)malloc(size_of_board);
  int q_size = 0, q_top = 0;
  int levels[MAX_BFS+1];
  memset(levels, 0, sizeof(int)*(MAX_BFS+1));
  int current_level = 0; int current_index = -1;
  Q_ENQUEUE(queue, q_size, root, size_of_board);   // enqueue first board
  levels[current_level]++; /* q_size == 1 */
  parents[q_top] = -1;
 
  if( levels[current_level] > max_level){
     num_result = 0;
     start_index = 0;
     return LLO_FAILED;
  }

  int pp, dd;
 
  while( q_size >= q_top){
    // dequeue a board, increment q_top
    PIN *curBoard = Q_DEQUEUE(queue, q_top);
    current_index++;
 
    // enqueue all its children
    for(pp = 0; pp < NUM_PINS; pp++){
      for(dd = 0; dd<4; dd++){
        // ignore same 'pp' and opposite 'dd': avoid go back
        if( pp == curBoard[NUM_PINS] && rid_int[dd] == curBoard[NUM_PINS+1] )
            continue;
        // if moved
        if( move_fp[dir_int[dd]](curBoard, pp, out) == LLO_SUCCEED){
          levels[current_level+1]++;
          VISIT(out, pp, dir_int[dd]);
          parents[q_size] = q_top - 1;                     // record the the queue index of its parent
          Q_ENQUEUE(queue, q_size, out, size_of_board);     // enqueue the new board
          if( out[0] == BOARD_CENTER ){                  // if found a solution
              max_level = current_level + 1;
              num_result = levels[current_level+1];
              start_index = q_size - levels[current_level+1];
    #ifdef RECORD_FIRST
              return LLO_SUCCEED;
    #endif
          }
        }
      } // for(dd = 0;
    } // for(pp = 0;

    if( current_index + 1 == levels[current_level] ){               // reached last board of current level, move to next level
      current_index = -1; current_level++;
      if( current_level + 1 >= max_level ){                   // reached the max_level, record the the last level of boards
        num_result = levels[current_level];                   // starting index of the last level in the queue
        start_index = q_size - levels[current_level];
        break;
      }
    }
  }
  return LLO_FAILED;
}

#endif // __LLO_BFS_H__