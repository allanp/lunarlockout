#ifndef LLO_INTERNAL_H_
#define LLO_INTERNAL_H_

#define LLO_OK  0
#define LLO_ERROR   -1

#define LLO_TRUE     1
#define LLO_FALSE    0

/* constant numbers */
static const int MAX_CHAR = 256;
static const int MAX_SIZE = (1 << 12);

#define MAX(x, y)        ( ( (x)>(y) ) ? (x) : (y) )
#define MIN(x, y)        ( ( (x)<(y) ) ? (x) : (y) )

/* directions */
#define UP      0
#define RIGHT   1
#define DOWN    2
#define LEFT    3
 
/* directions */
const char dir_char[4] = {'U', 'R', 'D', 'L'};    /* directions characters */
const int  dir_int[4]  = {UP, RIGHT, DOWN, LEFT}; /* directions integers   */

typedef short int PIN;

typedef struct _board *board_ptr;
typedef struct _board_result *board_result_ptr;

typedef struct _board{
	int board_width;
	int ctn_pins;
	PIN* pins;
};

typedef struct _board_result{
	int board_width;
	int ctn_pins;
	PIN* begin_pins;
	PIN* final_pins;
	int ctn_steps;
	char* steps;
};

/* move functions  */

int move_up	(board_ptr brd, int i, PIN out[]);
int move_right(board_ptr brd, int i, PIN out[]);
int move_down (board_ptr brd, int i, PIN out[]);
int move_left (board_ptr brd, int i, PIN out[]);

int (*const move_fp[4])(board_ptr , int , PIN []  ) = { move_up, move_right, move_down, move_left };

// function arrary
//typedef int (move_function)(board_ptr /* inputBoard */, 
//							int /* pinIndex */ , 
//							PIN [] /* outputBoard */ );
//
//move_function move_up, move_right, move_down, move_left;
//
//move_function move_fp[4] = {move_up, move_right, move_down, move_left };

#include <stdio.h>
#include <string.h>
#include <malloc.h>

#include "llo.h"


#endif // LLO_INTERNAL_H_