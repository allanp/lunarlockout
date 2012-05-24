#ifndef LLO_CPP_
#define LLO_CPP_

#include "include/timer.h"

#include "include/llo_internal.h"
#include "include/llo.h"

#include "include/llo_board.h"
#include "include/llo_solver.h"

void llo_init(){
#if defined(_WIN32)
	timer_init();
#endif
}

void llo_main(int argc, char** argv){

	board_ptr input;
	board_result_ptr *output;
	int ctn_output = 0;

	llo_solve(input, 4, 10, output, ctn_output);



}

#endif // LLO_CPP_