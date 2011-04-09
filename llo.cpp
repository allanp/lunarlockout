/// llo.cpp
/*************************************** README *************************************/
/****
 * Name: Tianhua Piao
 * Project: Lunar Lockout
 * Email: tianhua [AT] student.chalmers.se
 * Date: March 17, 2011
 *
 */
 
/************************************** DEFINES *************************************/
/* commands */

#define RECORD_FIRST

#define NUM_PINS        6
#define BOARD_WIDTH     5

#define MAX_DFS         20
#define MAX_BFS         4


/* board properties */
#define BOARD_MAX       ((BOARD_WIDTH * BOARD_WIDTH) - 1)
#define BOARD_CENTER    ((BOARD_WIDTH * BOARD_WIDTH) >> 1)
 
#define iPIN            (NUM_PINS)
#define iDIR            (NUM_PINS+1)
 
#define MAX_MOVES       (MAX_BFS + MAX_DFS)
 
/* directions */
#define UP      0
#define RIGHT   1
#define DOWN    2
#define LEFT    3
 
/* constant numbers */
#define MAX_CHAR     256
#define MAX_SIZE     (1<<16)
 
#define LLO_TRUE     1
#define LLO_FALSE    0
 
#define LLO_SUCCEED  0
#define LLO_FAILED   -1


/************************************** INCLUDES ************************************/
#include <sys/time.h>
 
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
 
 
/*************************************** MACROS *************************************/
#define MAX(x,y)        ( ( (x)>(y) ) ? (x) : (y) )
#define MIN(x,y)        ( ( (x)<(y) ) ? (x) : (y) )
 
#define FREE(x)         if( x != NULL ){ free(x); } x = NULL
 
#define VISIT(BOARD, PIN, DIR)                                 BOARD[iPIN] = (PIN); BOARD[iDIR] = (DIR)
#define Q_ENQUEUE(QUEUE, Q_SIZE, BOARD, BOARD_SIZE_IN_BYTES)   memcpy(QUEUE[(Q_SIZE)++], BOARD, BOARD_SIZE_IN_BYTES)
#define Q_DEQUEUE(QUEUE, Q_TOP)                                QUEUE[(Q_TOP)++]
 
#define S_PUSH(STACK, BOARD, S_SIZE, BOARD_SIZE_IN_BYTES)      memcpy( (STACK[(++S_SIZE)]), (BOARD), (BOARD_SIZE_IN_BYTES))
#define S_PEEK(STACK, S_SIZE)                                  STACK[(S_SIZE)]
#define S_POP(STACK, S_SIZE)                                   STACK[(S_SIZE)--]
 
/************************************** TYPEDEFS ************************************/
typedef short int PIN;

/* counters */
int total_solution;
 
/* boards */
PIN init_boards[MAX_SIZE][NUM_PINS];
PIN final_boards[MAX_SIZE][NUM_PINS];
PIN all_movements[MAX_SIZE][MAX_MOVES][2];

/* directions */
const char dirChar[4] = {'U', 'R', 'D', 'L'};    /* directions characters */
const int  dirInt[4] = {UP, RIGHT, DOWN, LEFT};  /* directions integers   */
const int  ridInt[4] = {DOWN, LEFT, UP, RIGHT};  /* opposite directions   */

/* move functions  */
int moveUP   ( PIN [] /* inputBoard */, int /* pinIndex */ , PIN [] /* outputBoard */ );
int moveRIGHT( PIN [] /* inputBoard */, int /* pinIndex */ , PIN [] /* outputBoard */ );
int moveDOWN ( PIN [] /* inputBoard */, int /* pinIndex */ , PIN [] /* outputBoard */ );
int moveLEFT ( PIN [] /* inputBoard */, int /* pinIndex */ , PIN [] /* outputBoard */ );
int (*const moveFP[4])(PIN [] /* inputBoard */,
                       int /* pinIndex */,
                       PIN [] /* outputBoard */ ) = { moveUP, moveRIGHT, moveDOWN, moveLEFT };

/************************************ MAIN ******************************************/

void createBoard(PIN p[], int boardIndex){

#if   NUM_PINS == 4   /* 4x4 */
  PIN temp[NUM_PINS] = {16, 6, 8, 23};
#elif NUM_PINS == 5   /* 5x5 */
    PIN temp[4][NUM_PINS] = {{6, 1, 2, 18, 21}, /* 6 */
                             {21, 0, 2, 4, 19}, /* Facebook, #40, 12 */
                             {6, 1, 2, 18, 21}, /* 6 */
                             {18, 0, 3, 4, 15}};  /* 8 */
#elif NUM_PINS == 6   /* 6x6 */
    PIN temp[1][NUM_PINS] = {/*{2, 0, 16, 18, 21, 23} */
//                             {19, 0, 1, 2, 5, 21},
                             {9, 0, 2, 4, 20, 24 }   /* 10 */
//                             {24, 0, 2, 4, 10, 20},   /* Facebook, #39, 13 */
//                             {21, 2, 4, 10, 11, 24},  /* Facebook, #38, 10 */
//                             {2, 7, 12, 14, 15, 23}, /* 7 */
                             }; /* wrong!!!!  */
#endif

  memcpy(p, temp[boardIndex], sizeof(PIN) * NUM_PINS );
}


int recordBoard(int boardIndex, int i, double t0, double t1){
  int minMoves = all_movements[boardIndex][0][0];
  fprintf(stdout, "\n");
  fprintf(stdout, "#%d:: Board #%d: %.3f msec {", boardIndex, i, t1 - t0);

  FILE *f = fopen("log.txt", "a");
  if(f== NULL) return -1;
  fprintf(f, "\nBoard #%d: {", i);

  fprintf(stdout, "%d", init_boards[boardIndex][0]);
  fprintf(f, "%d", init_boards[boardIndex][0]);
  for(int i = 1; i<NUM_PINS; i++){
    fprintf(stdout, ", %d", init_boards[boardIndex][i]);
    fprintf(f, ", %d", init_boards[boardIndex][i]);
  }

  if( minMoves < 10){
    fprintf(stdout, "} \nmove [0%d] {", minMoves);
    fprintf(f, "} \nmove [0%d] {", minMoves);
  }
  else{
    fprintf(stdout, "} \nmove [%d] {", minMoves);
    fprintf(f, "} \nmove [%d] {", minMoves);
  }

  fprintf(stdout, "%d%c", all_movements[boardIndex][1][0], dirChar[all_movements[boardIndex][1][1]]);
  fprintf(f, "%d%c", all_movements[boardIndex][1][0], dirChar[all_movements[boardIndex][1][1]]);
 
  for(int i = 2; i<minMoves + 1;i++){
    fprintf(stdout, "->%d%c", all_movements[boardIndex][i][0], dirChar[all_movements[boardIndex][i][1]]);
    fprintf(f, "->%d%c", all_movements[boardIndex][i][0], dirChar[all_movements[boardIndex][i][1]]);
  }
 
  fprintf(stdout, "}\nBoard: {%d", final_boards[boardIndex][0]);
  fprintf(f, "}\nBoard: {%d", final_boards[boardIndex][0]);
 
  for(int i = 1; i<NUM_PINS; i++){
    fprintf(stdout, ", %d", final_boards[boardIndex][i]);
    fprintf(f, ", %d", final_boards[boardIndex][i]);
  }

  fprintf(stdout, "}\n\n");
  fprintf(f, "}\n\n");
  ++boardIndex;
  fclose(f);
  return 0;
}

 
/***************************** FUNCTIONS IMPLEMENTATIONS ****************************/
 
/* initialization functions */
void init_all(){
  memset(init_boards, (PIN)-1, sizeof(PIN) * NUM_PINS * MAX_SIZE);
  memset(final_boards, (PIN)-1, sizeof(PIN) * NUM_PINS * MAX_SIZE);
  total_solution = 0;
}
 
void init(){ };
 
/* timing functions */
#ifdef _WIN32
void init_timer(){
  QueryPerformanceFrequency((LARGE_INTEGER *)&_freq);
}
 
void mytimer(double *et /* microseconds */ ){
  __int64 t;
  QueryPerformanceCounter((LARGE_INTEGER *)&t);
  *et = (double)t * 1000 / (double)_freq;
}
#else
void mytimer(double *et /* microseconds */ ){
  struct timeval t;
  gettimeofday( &t, (struct timezone *)0 );
  *et = t.tv_sec*1.0e3 + t.tv_usec*1.0e-3;
}
#endif
 
/** move functions */
int moveUP(PIN p[], int i, PIN out[]){
  int row = p[i] / BOARD_WIDTH;
  int col = p[i] % BOARD_WIDTH;
  int min_distance = BOARD_WIDTH;
  int ii;
  for(ii = 0; ii<NUM_PINS; ii++){
    if( ii == i ) continue;
    int other_row = p[ii] / BOARD_WIDTH;
    int other_col = p[ii] % BOARD_WIDTH;
    if( other_col != col ) continue;
    if( other_row > row ) continue;
    min_distance = MIN(row - other_row - 1, min_distance);
  }
  if( min_distance != BOARD_WIDTH && min_distance > 0){
    memcpy(out, p,sizeof(PIN)*NUM_PINS);
    out[i] = p[i] - min_distance * BOARD_WIDTH;
    return LLO_SUCCEED;
  }
  return LLO_FAILED;
}

int moveDOWN(PIN p[], int i, PIN out[]){
  int row = p[i] / BOARD_WIDTH;
  int col = p[i] % BOARD_WIDTH;
  int min_distance = BOARD_WIDTH;
  int ii;
  for(ii = 0; ii<NUM_PINS; ii++){
    if( ii == i ) continue;
    int other_row = p[ii] / BOARD_WIDTH;
    int other_col = p[ii] % BOARD_WIDTH;
    if( other_col != col ) continue;
    if( other_row < row ) continue;
    min_distance = MIN(other_row - row - 1, min_distance);
  }
  if( min_distance != BOARD_WIDTH && min_distance > 0){
    memcpy(out, p,sizeof(PIN)*NUM_PINS);
    out[i] = p[i] + min_distance * BOARD_WIDTH;
    return LLO_SUCCEED;
  }
  return LLO_FAILED;
}

int moveLEFT(PIN p[], int i, PIN out[]){
  int row,col;
  row = p[i]/BOARD_WIDTH;
  col = p[i]%BOARD_WIDTH;
  int min_distance = BOARD_WIDTH;
  int ii;
  for(ii = 0; ii<NUM_PINS; ii++){
    if( ii == i ) continue;
    int other_row = p[ii] / BOARD_WIDTH;
    int other_col = p[ii] % BOARD_WIDTH;
    if( other_row != row ) continue;
    if( other_col > col ) continue;
    min_distance = MIN(col - other_col - 1, min_distance);
  }
  if( min_distance != BOARD_WIDTH && min_distance > 0){
    memcpy(out, p,sizeof(PIN)*NUM_PINS);
    out[i] = p[i] - min_distance;
    return LLO_SUCCEED;
  }

  return LLO_FAILED;
}

int moveRIGHT(PIN p[], int i, PIN out[]){
  int row,col;    fprintf(stdout, "\r"); fflush(stdout);
  row = p[i] / BOARD_WIDTH;
  col = p[i] % BOARD_WIDTH;
  int min_distance = BOARD_WIDTH;
  int ii;
  for(ii = 0; ii<NUM_PINS; ii++){
    if( ii == i ) continue;
    int other_row = p[ii] / BOARD_WIDTH;
    int other_col = p[ii] % BOARD_WIDTH;
    if( other_row != row ) continue;
    if( other_col < col ) continue;
    min_distance = MIN(other_col - col - 1, min_distance);
  }
  if( min_distance != BOARD_WIDTH && min_distance > 0){
    memcpy(out, p,sizeof(PIN)*NUM_PINS);
    out[i] = p[i] + min_distance;
    return LLO_SUCCEED;
  }
  return LLO_FAILED;
}

/* board operations functions */
void combineMoves(PIN queue[][NUM_PINS+2], int parents[], int idxBFS, int bfsMoves, PIN movements[][2]){
  // record the movement history reverse order
  while( idxBFS > 0 && bfsMoves > 0){
      --bfsMoves;
      if( bfsMoves < 0) break;
      movements[bfsMoves][0] = queue[idxBFS][iPIN];
      movements[bfsMoves][1] = queue[idxBFS][iDIR];
      idxBFS = parents[idxBFS];
  }
}


int solveBFS(PIN* root, int& maxLevel, PIN queue[][NUM_PINS+2], int* parents, int& startIndex, int& numResult){

  if(root[0] == BOARD_CENTER){
    startIndex = 0;
    numResult = 1;    
    return LLO_SUCCEED;
  }

  size_t sizeOfBoard = sizeof(PIN) * (NUM_PINS + 2);

  PIN * out = (PIN*)malloc(sizeOfBoard);
  int qSize = 0, qTop = 0;
  int levels[MAX_BFS+1];
  memset(levels, 0, sizeof(int)*(MAX_BFS+1));
  int curLevel = 0; int curIdx = -1;
  Q_ENQUEUE(queue, qSize, root, sizeOfBoard);   // enqueue first board
  levels[curLevel]++; /* qSize == 1 */
  parents[qTop] = -1;
 
  if( levels[curLevel] > maxLevel){
     numResult = 0;
     startIndex = 0;
     return LLO_FAILED;
  }

  int pp, dd;
 
  while( qSize >= qTop){
    // dequeue a board, increment qTop
    PIN *curBoard = Q_DEQUEUE(queue, qTop);
    curIdx++;
 
    // enqueue all its children
    for(pp = 0; pp < NUM_PINS; pp++){
      for(dd = 0; dd<4; dd++){
        // ignore same 'pp' and opposite 'dd': avoid go back
        if( pp == curBoard[NUM_PINS] && ridInt[dd] == curBoard[NUM_PINS+1] )
            continue;
        // if moved
        if( moveFP[dirInt[dd]](curBoard, pp, out) == LLO_SUCCEED){
          levels[curLevel+1]++;
          VISIT(out, pp, dirInt[dd]);
          parents[qSize] = qTop - 1;                     // record the the queue index of its parent
          Q_ENQUEUE(queue, qSize, out, sizeOfBoard);     // enqueue the new board
          if( out[0] == BOARD_CENTER ){                  // if found a solution
              maxLevel = curLevel + 1;
              numResult = levels[curLevel+1];
              startIndex = qSize - levels[curLevel+1];
    #ifdef RECORD_FIRST
              return LLO_SUCCEED;
    #endif
          }
        }
      } // for(dd = 0;
    } // for(pp = 0;

    if( curIdx + 1 == levels[curLevel] ){               // reached last board of current level, move to next level
      curIdx = -1; curLevel++;
      if( curLevel + 1 >= maxLevel ){                   // reached the maxLevel, record the the last level of boards
        numResult = levels[curLevel];                   // starting index of the last level in the queue
        startIndex = qSize - levels[curLevel];
        break;
      }
    }
  }
  return LLO_FAILED;
}

int solveDFS(PIN* board, PIN* result, PIN movements[][2], int& moves, const int& startIndex){
 
  // some initialization setup
  PIN boards[MAX_MOVES+1][NUM_PINS+2]; // {P[0]...P[NUM_PINS-1] + PinIdx + Direction}
 
  PIN *p, *out;
 
  int pp = 0;   // pin index [0 ~ NUM_PINS-1]
  int dd = UP;  // UP = 0(^), RIGHT = 1(>), DOWN = 2(v), LEFT = 3(<)
  int moved = LLO_FAILED;
  size_t sizeOfBoard = sizeof(PIN) * (NUM_PINS+2);
 
  out = (PIN*)malloc(sizeOfBoard);
  memset(boards, (PIN)-1, sizeOfBoard * MAX_MOVES );
  int size = -1;
  int max_step = moves;
  int solution = 0;
 
  int mark_p, mark_d;

  // push board into stack
  S_PUSH(boards, board, size, sizeOfBoard);
 
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
    else if( p[iPIN] == mark_p && p[iDIR] == ridInt[mark_d]) 
      moved = LLO_FAILED;
    else
      moved = (p[iDIR] < 4) ? moveFP[p[iDIR]](p,  p[iPIN], out) : LLO_FAILED;

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
      S_PUSH(boards, out, size, sizeOfBoard);
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
            mark_p=boards[size-1][iPIN]; mark_d=boards[size-1][iDIR];
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

/* solver functions */
int trySolve(PIN p[], int boardIndex){
  int rc = 0;                              /* return value */

  PIN finalResult[NUM_PINS+2];             /* final board */
  int minMoves = MAX_MOVES;                /* the minimal Moves */
  int solution = 0;

  PIN root[NUM_PINS+2];                    /* first board */
  memcpy(root, p, sizeof(PIN) * NUM_PINS); /* the starting board */
  VISIT(root, 0, -1);                      /* mark the moving pin and direction, start from p0, d0 */

  int max_levels = MAX_BFS;                /* MAX_BFS; */
  
  PIN queue[MAX_SIZE][NUM_PINS+2];         /* queue used to store the bfs-tree */
  memset(queue, (PIN)-1, sizeof(PIN) * MAX_SIZE * (NUM_PINS+2));

  int parents[MAX_SIZE];                   /* record i-th element's parents in the queue */
  memset(parents, (int)-1, sizeof(int)*MAX_SIZE);

  int bfsIndex = 0;                        /*  */

  int numResult = 0;                       /* number of boards in the last level in bfs */
  
  PIN movements[MAX_MOVES][2];             /* store the solution movements */
  memset(movements, (PIN)-1, sizeof(PIN)*MAX_MOVES*2);
 
  rc = solveBFS(root, max_levels, queue, parents, bfsIndex, numResult);  /* solve bfs-ly */

  if(rc != LLO_FAILED ){                   /* found a solution in the bfs */
    minMoves = max_levels;
    solution++;
    int i;
    // find the result board from the queue (from the last level)
    for(i=0;i<numResult;++i){
      if(queue[i+bfsIndex][0] != BOARD_CENTER )
        continue;
      /* record the final board */
      memcpy(final_boards[boardIndex], queue[i+bfsIndex], sizeof(PIN) * (NUM_PINS+2));
      memcpy(init_boards[boardIndex], root, sizeof(PIN)*NUM_PINS);
      /* record the movement history */
      combineMoves(queue, parents, i+bfsIndex, minMoves, movements);              
      all_movements[boardIndex][0][0] = minMoves;
      memcpy(all_movements[boardIndex][1], movements, sizeof(PIN)*minMoves*2);
      break;
    }
    return LLO_SUCCEED;
  }

  /* do the dfs on the each board of last #numResult# boards in the queue */
  PIN * dfsResult = (PIN*)malloc(sizeof(PIN) * (NUM_PINS + 2));

  int dfsMoves = MAX_DFS;       /* number moves in dfs */
  int bfsMoves = max_levels - 1;
  
  int startIndex = bfsIndex;      /* the start index in the queue of last level of bfs results */

  while( --numResult >= 0 ){

    /* solve dfs-ly */
    rc = solveDFS(queue[startIndex + numResult], dfsResult, &movements[max_levels-1], dfsMoves, numResult);
    
    if( rc == LLO_FAILED )
      continue;
    
    ++solution;
    
    if( dfsMoves >= minMoves )
      continue;
      
    solution = 1;
    minMoves = dfsMoves;
    bfsIndex = startIndex + numResult;
    memcpy(finalResult, dfsResult, sizeof(PIN) * (NUM_PINS + 2));
  }
  dfsMoves = minMoves;
 
  if( solution == 0 )
    return LLO_FAILED;

  memcpy(final_boards[boardIndex], finalResult, sizeof(PIN) * NUM_PINS);
  memcpy(init_boards[boardIndex], root, sizeof(PIN)*NUM_PINS);

  combineMoves(queue, parents, bfsIndex, bfsMoves, movements);

  all_movements[boardIndex][0][0] = dfsMoves + bfsMoves;
  memcpy(all_movements[boardIndex][1], movements, sizeof(PIN)*(MAX_MOVES)*2);

  /* initBoard  := root        [ppp....PD]   */
  /* finalBoard := finalResult [ppp....PD]   */
  /* movements  := movements   [pdpdpd...]   */

  return LLO_SUCCEED;
}


int main(int argc, char** argv){
 
  double t0, t1;
  int boardIndex = 0;
  fprintf(stdout, "NUM_PINS: %d, BOARD_WIDTH: %d, BFS: %d, DFS: %d -> \n\n", NUM_PINS, BOARD_WIDTH, MAX_BFS, MAX_DFS);

  int total = 1;

#ifdef _WIN32
  init_timer();
#endif

  FILE *f = fopen("log.txt", "w");
  if(f== NULL) return -1;
  fclose(f);

  PIN board[NUM_PINS];
  PIN root[NUM_PINS];
  memset(board, (PIN)-1, sizeof(PIN)*NUM_PINS);
  memset(root, (PIN)-1, sizeof(PIN)*NUM_PINS);

  double totalTime = 0, totalFoundTime = 0;
  long totalFoundCounter = 0;
  int i = 0;

  while(i < total ){
    if(boardIndex > MAX_SIZE)break;
    createBoard(board, boardIndex);
    i++;
    if(board[0] == BOARD_CENTER)       /* already the final board */
      continue;
    int rc;
    fprintf(stdout, "solving board #%d...\r", i); fflush(stdout);
    mytimer(&t0);
    rc = trySolve(board, boardIndex);
    mytimer(&t1);

    totalTime += t1 - t0;

    if( rc == LLO_SUCCEED){
      totalFoundTime += t1 - t0;
      totalFoundCounter++;
//      recordBoard(boardIndex, i, t0, t1);
    }
  }

  f = fopen("log.txt", "a");
  if(f== NULL)
    return -1;
    
  fprintf(f, "\nTotal Time: %.3f msec, Found Time: %.3f msec, Found: %ld\n", totalTime, totalFoundTime, totalFoundCounter);
  fclose(f);
  fprintf(stdout, "\nTotal Time: %.3f msec, Found Time: %.3f msec, Total Found Counter: %ld\n", totalTime, totalFoundTime, totalFoundCounter);
  fprintf(stdout, "finished program.\n");

  getchar();

  return 0;
}

