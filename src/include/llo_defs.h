#ifndef LLO_DEFS_H_
#define LLO_DEFS_H_

/************************************** DEFINES *************************************/

static const int version_major = 0;
static const int version_minor = 1;

/* board properties */
#define BOARD_MAX       ((BOARD_WIDTH * BOARD_WIDTH) - 1)
#define BOARD_CENTER    ((BOARD_WIDTH * BOARD_WIDTH) >> 1)
 
#define iPIN            (NUM_PINS)
#define iDIR            (NUM_PINS + 1)
 
#define MAX_MOVES       (MAX_BFS + MAX_DFS)

/* directions */
#define UP      0
#define RIGHT   1
#define DOWN    2
#define LEFT    3
 
/* constant numbers */
#define MAX_CHAR     256
#define MAX_SIZE     (1 << 12)
 
#define LLO_TRUE     1
#define LLO_FALSE    0
 
#define LLO_SUCCEED  0
#define LLO_FAILED   -1

/************************************** INCLUDES ************************************/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/************************************** TYPEDEFS ************************************/

typedef short int PIN;

typedef struct _board{
	int board_width;
	int ctn_pins;
	PIN* pins;
}board, *board_ptr;

typedef struct _board_result{
	int ctn_pins;
	int board_width;

	PIN* begin_pins;
	PIN* final_pins;

	int ctn_steps;
	char* steps;
}board_result, *board_result_ptr;

const char* log_filename = "log.txt";

/*************************************** MACROS *************************************/
#define MAX(x, y)        ( ( (x)>(y) ) ? (x) : (y) )
#define MIN(x, y)        ( ( (x)<(y) ) ? (x) : (y) )
 
#define FREE(x)         if( x != NULL ){ free(x); } x = NULL
 
#define VISIT(BOARD, PIN, DIR)                                 BOARD[iPIN] = (PIN); BOARD[iDIR] = (DIR)
#define Q_ENQUEUE(QUEUE, Q_SIZE, BOARD, BOARD_SIZE_IN_BYTES)   memcpy(QUEUE[(Q_SIZE)++], BOARD, BOARD_SIZE_IN_BYTES)
#define Q_DEQUEUE(QUEUE, Q_TOP)                                QUEUE[(Q_TOP)++]
 
#define S_PUSH(STACK, BOARD, S_SIZE, BOARD_SIZE_IN_BYTES)      memcpy( (STACK[(++S_SIZE)]), (BOARD), (BOARD_SIZE_IN_BYTES))
#define S_PEEK(STACK, S_SIZE)                                  STACK[(S_SIZE)]
#define S_POP(STACK, S_SIZE)                                   STACK[(S_SIZE)--]

/* counters */
int total_solution;
 
/* boards */
PIN init_boards[MAX_SIZE][NUM_PINS];
PIN final_boards[MAX_SIZE][NUM_PINS];
PIN all_movements[MAX_SIZE][MAX_MOVES][2];

/* directions */
const char dir_char[4] = {'U', 'R', 'D', 'L'};    /* directions characters */
const int  dir_int[4]  = {UP, RIGHT, DOWN, LEFT}; /* directions integers   */
const int  rid_int[4]  = {DOWN, LEFT, UP, RIGHT}; /* opposite directions   */

/* move functions  */
int move_up   ( PIN [] /* inputBoard */, int /* pinIndex */ , PIN [] /* outputBoard */ );
int move_right( PIN [] /* inputBoard */, int /* pinIndex */ , PIN [] /* outputBoard */ );
int move_down ( PIN [] /* inputBoard */, int /* pinIndex */ , PIN [] /* outputBoard */ );
int move_left ( PIN [] /* inputBoard */, int /* pinIndex */ , PIN [] /* outputBoard */ );

int (*const move_fp[4])(PIN [] /* inputBoard */,
                       int /* pinIndex */,
                       PIN [] /* outputBoard */ ) = { move_up, move_right, move_down, move_left };

#include "llo_moves.h"

#endif // LLO_DEFS_H_
