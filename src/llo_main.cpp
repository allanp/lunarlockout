/****
 * Name: allanp
 * Project: Lunar Lockout
 * Email: allanp.cocoa [AT] gmail.com
 * Date: May 20, 2011
 */

#ifndef LLO_MAIN_CPP_
#define LLO_MAIN_CPP_

#include "include\llo.h"

int main(int argc, char** argv){
  
  llo_main(argc, argv);

  fprintf(stdout, "LLO has finished.\n");

  getchar();

  return 0;
}

#endif // LLO_MAIN_CPP_
