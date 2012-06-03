#ifndef LLO_SOLVER_CPP_
#define LLO_SOLVER_CPP_

#include "include/llo_solver.h"
#include "include/llo_board.h"

int solve_bfs(board_ptr input, int max_bfs, board_result_ptr *bfs_results, int &ctn_bfs_results){
	if(!input->pins) 
		return LLO_ERROR;

	ctn_bfs_results = 0;
	bfs_results = NULL;

	if(input->pins[0] == BOARD_CENTER(input->board_width)){
		ctn_bfs_results = 1;

		board_result_init(bfs_results[0], input->board_width, input->ctn_pins, 0);
		memcpy(bfs_results[0]->begin_pins, input->pins, BOARD_SIZE_BYTES(input->ctn_pins));
		memcpy(bfs_results[0]->final_pins, input->pins, BOARD_SIZE_BYTES(input->ctn_pins));
		bfs_results[0]->ctn_steps = 0;
		bfs_results[0]->steps = NULL;
		return LLO_OK;
	}

	// use a queue

	int max_bfs_boards = board_max_ctn(input->ctn_pins, max_bfs);
	PIN *current_board;
	int pos = 0;
	int board_size = sizeof(PIN) * input->ctn_pins;
	PIN *queue = (PIN*)malloc(board_size * max_bfs_boards);
	
	// enqueue :
	memcpy(queue, input->pins, board_size);
	// peek    :
	current_board = &queue[pos];
	// dequeue : peek - 1
	






	return LLO_OK;
}

int solve_dfs(board_ptr input, int max_dfs, board_result_ptr *dfs_results, int &ctn_dfs_results){
	if(!input->pins) return LLO_ERROR;

	// use a stack


	return LLO_OK;
}

int llo_solve(const board_ptr input, const int max_bfs, const int max_dfs, board_result_ptr *output, int &ctn_output){
	if(!input)
		return LLO_ERROR;
	
	ctn_output = 0;
	
	// solve bfs-ly
	int ctn_bfs_results = 0;
	board_result_ptr *bfs_results;

	solve_bfs(input, max_bfs, bfs_results, ctn_bfs_results);
	if(ctn_bfs_results > 0){
		// solved in bfs
		memcpy(output, bfs_results, sizeof(board_result_ptr) * ctn_bfs_results);
		ctn_output = ctn_bfs_results;
		return LLO_OK;
	}

	// for each result board of bfs-results
	// bi: bfs_index
	for(int bi = 0; bi < ctn_bfs_results; bi++){
		// solve dfs-ly
		if(!bfs_results[bi]) break;

		board_ptr dfs_input;
		board_result_final_to_board(bfs_results[bi], dfs_input);

		board_result_ptr *dfs_results;
		
		int ctn_dfs_results = 0;
		solve_dfs(dfs_input, max_dfs, dfs_results, ctn_dfs_results);
		if(ctn_dfs_results > 0){
			// solved in dfs
#if defined(LLO_ONE_RESULT)
			// record only the first result
			output = (board_result_ptr*)malloc(sizeof(board_result_ptr));
			board_result_merge(bfs_results[bi], dfs_results[0], output[0]);
			ctn_output = 1;
			break;
#else
			// record all results
			fprintf(stderr, "not implemented yet.\n");
			exit(LLO_ERROR);
#endif
		}
		board_destroy(dfs_input);
	}
	return LLO_OK;
}

#endif // LLO_SOLVER_CPP_