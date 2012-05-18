#ifndef __LLO_H__
#define __LLO_H__

#define RECORD_FIRST

#define NUM_PINS        6
#define BOARD_WIDTH     5

#define MAX_DFS         10
#define MAX_BFS         4

#include "timer.h"

#include "llo_defs.h"

#include "llo_dfs.h" // depth-first search
#include "llo_bfs.h" // breadth-first search

/* initialization functions */
void init_all(){
  const int max_size = board_size_in_bytes * MAX_SIZE;
  memset(init_boards, (PIN)-1, max_size);
  memset(final_boards, (PIN)-1, max_size);
  total_solution = 0;
}

void create_board(PIN board[], const char* filename, int* line_num){
  
  char line[80];
  char *ptr = 0;
  int i = 0;
  
  FILE *f = fopen(filename, "r");
  if(!f){
    *line_num = -1;
    return;
  }

  int current_line = 0;

  while( fgets(line, sizeof(line), f) != NULL)
  {
     // clear the array
     memset(board,0,sizeof(board));
     i = 0;
     // extract numbers from line string
     ptr = strtok(line," ");
     while( ptr != NULL)
     {
       // insert into array
       board[i] = atoi(ptr);
       ++i;
       // get next number from the string
       ptr = strtok(NULL, " ");
     }
     if(current_line == *line_num){
       fclose(f);
       // reached the line
       return;
     }
     current_line++;
  }

  *line_num = -1;
  fclose(f);
}

void create_board(PIN p[], int board_index){

#if   NUM_PINS == 4   /* 4x4 */
  PIN temp[NUM_PINS] = {16, 6, 8, 23};
#elif NUM_PINS == 5   /* 5x5 */
    PIN temp[4][NUM_PINS] = {{6, 1, 2, 18, 21}, /* 6 */
                             {21, 0, 2, 4, 19}, /* Facebook, #40, 12 */
                             {6, 1, 2, 18, 21}, /* 6 */
                             {18, 0, 3, 4, 15}};  /* 8 */
#elif NUM_PINS == 6   /* 6x6 */
    PIN temp[1][NUM_PINS] = {
//                            { 2, 0, 16, 18, 21, 23},  /* 2 */
//                            {19, 0,  1,  2,  5, 21},  /* 13 */
//                            { 9, 0,  2,  4, 20, 24}   /* 10 */
//                            {24, 0,  2,  4, 10, 20},  /* Facebook, #39, 13 */
                              {21, 2,  4, 10, 11, 24},  /* Facebook, #38, 10 */
//                            { 2, 7, 12, 14, 15, 23},  /* 7 */
                            };
#endif
  memcpy(p, temp[board_index], board_size_in_bytes );
}

int record_board(int board_index, int i, double t0, double t1){
  int min_moves = all_movements[board_index][0][0];
  fprintf(stdout, "\n");
  fprintf(stdout, "#%d:: Board #%d: %.3f msec {", board_index, i, t1 - t0);

  FILE *f = fopen("log.txt", "a");
  if(f== NULL) return -1;
  fprintf(f, "\nBoard #%d: {", i);

  // fprintf(stdout, "%d", init_boards[board_index][0]);
  fprintf(f, "%d", init_boards[board_index][0]);
  for(int i = 1; i<NUM_PINS; i++){
    // fprintf(stdout, ", %d", init_boards[board_index][i]);
    fprintf(f, ", %d", init_boards[board_index][i]);
  }

  if( min_moves < 10){
    // fprintf(stdout, "} \nmove [0%d] {", min_moves);
    fprintf(f, "} \nmove [0%d] {", min_moves);
  }
  else{
    // fprintf(stdout, "} \nmove [%d] {", min_moves);
    fprintf(f, "} \nmove [%d] {", min_moves);
  }

  // fprintf(stdout, "%d%c", all_movements[board_index][1][0], dir_char[all_movements[board_index][1][1]]);
  fprintf(f, "%d%c", all_movements[board_index][1][0], dir_char[all_movements[board_index][1][1]]);
 
  for(int i = 2; i<min_moves + 1;i++){
    // fprintf(stdout, "->%d%c", all_movements[board_index][i][0], dir_char[all_movements[board_index][i][1]]);
    fprintf(f, "->%d%c", all_movements[board_index][i][0], dir_char[all_movements[board_index][i][1]]);
  }
 
  // fprintf(stdout, "}\nBoard: {%d", final_boards[board_index][0]);
  fprintf(f, "}\nBoard: {%d", final_boards[board_index][0]);
 
  for(int i = 1; i<NUM_PINS; i++){
    // fprintf(stdout, ", %d", final_boards[board_index][i]);
    fprintf(f, ", %d", final_boards[board_index][i]);
  }

  // fprintf(stdout, "}\n\n");
  fprintf(f, "}\n\n");
  ++board_index;
  fclose(f);
  return 0;
}

/* board operations functions */
void combine_moves(PIN queue[][NUM_PINS+2], int parents[], int index_bfs, int bfs_moves, PIN movements[][2]){
  // record the movement history reverse order
  while( index_bfs > 0 && bfs_moves > 0){
      --bfs_moves;
      if( bfs_moves < 0) break;
      movements[bfs_moves][0] = queue[index_bfs][iPIN];
      movements[bfs_moves][1] = queue[index_bfs][iDIR];
      index_bfs = parents[index_bfs];
  }
}

/* solver functions */
int try_solve(PIN p[], int board_index){
  int rc = 0;                              /* return value */

  PIN final_result[NUM_PINS+2];             /* final board */
  int min_moves = MAX_MOVES;                /* the minimal Moves */
  int solution = 0;

  PIN root[NUM_PINS+2];                    /* first board */
  memcpy(root, p, board_size_in_bytes);    /* the starting board */
  VISIT(root, 0, -1);                      /* mark the moving pin and direction, start from p0, d0 */

  int max_levels = MAX_BFS;                /* MAX_BFS; */
  
  PIN queue[MAX_SIZE][NUM_PINS+2];         /* queue used to store the bfs-tree */
  memset(queue, (PIN)-1, sizeof(PIN) * MAX_SIZE * (NUM_PINS+2));

  int parents[MAX_SIZE];                   /* record i-th element's parents in the queue */
  memset(parents, (int)-1, sizeof(int)*MAX_SIZE);

  int bfs_index = 0;                        /*  */

  int num_result = 0;                       /* number of boards in the last level in bfs */
  
  PIN movements[MAX_MOVES][2];             /* store the solution movements */
  memset(movements, (PIN)-1, sizeof(PIN)*MAX_MOVES*2);
 
  rc = solve_bfs(root, max_levels, queue, parents, bfs_index, num_result);  /* solve bfs-ly */

  if(rc != LLO_FAILED ){                   /* found a solution in the bfs */
    min_moves = max_levels;
    solution++;
    int i;
    // find the result board from the queue (from the last level)
    for(i=0;i<num_result;++i){
      if(queue[i+bfs_index][0] != BOARD_CENTER )
        continue;
      /* record the final board */
      memcpy(final_boards[board_index], queue[i+bfs_index], sizeof(PIN) * (NUM_PINS+2));
      memcpy(init_boards[board_index], root, board_size_in_bytes);
      /* record the movement history */
      combine_moves(queue, parents, i+bfs_index, min_moves, movements);              
      all_movements[board_index][0][0] = min_moves;
      memcpy(all_movements[board_index][1], movements, sizeof(PIN)*min_moves*2);
      break;
    }
    return LLO_SUCCEED;
  }

  /* do the dfs on the each board of last #num_result# boards in the queue */
  PIN * dfs_result = (PIN*)malloc(sizeof(PIN) * (NUM_PINS + 2));

  int dfs_moves = MAX_DFS;       /* number moves in dfs */
  int bfs_moves = max_levels - 1;
  
  int start_index = bfs_index;      /* the start index in the queue of last level of bfs results */

  while( --num_result >= 0 ){

    /* solve dfs-ly */
    rc = solve_dfs(queue[start_index + num_result], dfs_result, &movements[max_levels-1], dfs_moves, num_result);
    
    if( rc == LLO_FAILED )
      continue;
    
    ++solution;
    
    if( dfs_moves >= min_moves )
      continue;
      
    solution = 1;
    min_moves = dfs_moves;
    bfs_index = start_index + num_result;
    memcpy(final_result, dfs_result, sizeof(PIN) * (NUM_PINS + 2));
  }
  dfs_moves = min_moves;
 
  if( solution == 0 )
    return LLO_FAILED;

  memcpy(final_boards[board_index], final_result, board_size_in_bytes);
  memcpy(init_boards[board_index], root, board_size_in_bytes);

  combine_moves(queue, parents, bfs_index, bfs_moves, movements);

  all_movements[board_index][0][0] = dfs_moves + bfs_moves;
  memcpy(all_movements[board_index][1], movements, sizeof(PIN)*(MAX_MOVES)*2);

  /* initBoard  := root        [ppp....PD]   */
  /* finalBoard := final_result [ppp....PD]   */
  /* movements  := movements   [pdpdpd...]   */

  return LLO_SUCCEED;
}

int llo_main(const char* filename){

  double t0, t1;
  int board_index = 0;
  fprintf(stdout, "NUM_PINS: %d, BOARD_WIDTH: %d, BFS: %d, DFS: %d -> \n\n", NUM_PINS, BOARD_WIDTH, MAX_BFS, MAX_DFS);

  int total = 1;

  FILE *f = fopen("log.txt", "w");
  if(f == NULL) return -1;
  fclose(f);

  PIN board[NUM_PINS];
  PIN root[NUM_PINS];
  memset(board, (PIN)-1, board_size_in_bytes);
  memset(root, (PIN)-1, board_size_in_bytes);

  double total_time = 0, total_found_time = 0;
  long total_found_counter = 0;
  int i = 0;
  int line = 0;

  int last_line = 0;
  while(true){ //i < total ){

    if(board_index > MAX_SIZE) 
      break;

    last_line = line;
    create_board(board, filename, &line);

    if(line == -1) 
      break;
    line++;

    i++;
    if(board[0] == BOARD_CENTER)       /* already the final board */
      continue;
    int rc;
    fprintf(stdout, "solving board #%d...\r", i); 
    fflush(stdout);
    
    timer(&t0);
    rc = try_solve(board, board_index);
    timer(&t1);

    total_time += t1 - t0;

    if( rc == LLO_SUCCEED){
      total_found_time += t1 - t0;
      total_found_counter++;
      record_board(board_index, i, t0, t1);
    }
  }

  f = fopen("log.txt", "a");
  if(f== NULL)
    return -1;
    
  fprintf(f, "\nTotal Time: %.3f msec, Found Time: %.3f msec, Found: %ld\n", total_time, total_found_time, total_found_counter);
  fclose(f);
  fprintf(stdout, "\nTotal Time: %.3f msec, Found Time: %.3f msec, Total Found Counter: %ld\n", total_time, total_found_time, total_found_counter);
  fprintf(stdout, "finished program.\n");

  return total;
}

#include "llo_board.h"

#endif // __LLO_H__