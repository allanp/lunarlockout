/// llo_main.c
/****
 * Name: Tianhua Piao
 * Project: Lunar Lockout
 * Email: tianhua [AT] student.chalmers.se
 * Date: May 17, 2011
 */

#include "include\llo.h"

int main(int argc, char** argv){
  
#ifdef _WIN32
  init_timer();
#endif

  int rc = 0;
  
  char filename[50];
  llo_create_boards_filename(BOARD_MAX + 1, NUM_PINS, filename);
  
  llo_create_boards_file(filename, true);
  llo_board(BOARD_MAX + 1, NUM_PINS, filename);
  
  rc = llo_main(filename);
  
  fprintf(stdout, "LLO has finished. return value: %d\n", rc);

  getchar();

  return 0;
}
