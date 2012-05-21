/****
 * Name: allanp
 * Project: Lunar Lockout
 * Email: allanp.cocoa [AT] gmail.com
 * Date: May 20, 2011
 */

#ifndef LLO_MAIN_CPP_
#define LLO_MAIN_CPP_

#define DEBUG_

#define RECORD_FIRST

static const int NUM_PINS = 8;
static const int BOARD_WIDTH = 7;

static const int MAX_BFS = 5;
static const int MAX_DFS = 10;

#include "include\llo.h"

int main(int argc, char** argv){
  
  init_timer();

  llo_main(argc, argv);

  fprintf(stdout, "LLO has finished.\n");

  getchar();

  return 0;
}

#endif // LLO_MAIN_CPP_
