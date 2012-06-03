#ifndef LLO_BOARD_CPP_
#define LLO_BOARD_CPP_

#include <malloc.h>

#include "include/llo_internal.h"
#include "include/llo_board.h"

int board_init(board_ptr brd, int board_width, int ctn_pins){
	if(!brd){
		brd = (board_ptr)malloc(sizeof(board_ptr));
		brd->board_width = board_width;
		brd->ctn_pins = ctn_pins;
		memset(brd->pins, (PIN)-1, BOARD_SIZE_BYTES(brd->ctn_pins));
	}
	return LLO_OK;
}

int board_copy(const board_ptr input, board_ptr output){
	if(!input || !input->pins) return LLO_ERROR;
	board_destroy(output);
	output = (board_ptr)malloc(sizeof(board_ptr));
	output->board_width = input->board_width;
	output->ctn_pins = input->ctn_pins;
	memcpy(output->pins, input->pins, BOARD_SIZE_BYTES(input->ctn_pins));

	return LLO_OK;
}
void board_destroy(board_ptr brd){
	if(!brd)
		return;
	if(brd->pins){
		free(brd->pins);
		brd->pins = NULL;
	}
	free(brd);
	brd = NULL;
}

int board_clear(board_ptr brd){
	if(!brd || !brd->pins)
		return LLO_ERROR;
	memset(brd->pins, (PIN)-1, BOARD_SIZE_BYTES(brd->ctn_pins));
	return LLO_OK;
}

int board_result_init(board_result_ptr brd, int board_width, int ctn_pins, int max_steps){
	if(!brd){
		brd = (board_result_ptr)malloc(sizeof(board_result_ptr));
		if(!brd) return LLO_ERROR;
		brd->begin_pins = (PIN*)malloc(BOARD_SIZE_BYTES(ctn_pins));
		if(!brd->begin_pins) return LLO_ERROR;
		brd->final_pins = (PIN*)malloc(BOARD_SIZE_BYTES(ctn_pins));
		if(!brd->final_pins) return LLO_ERROR;

		brd->ctn_pins = ctn_pins;
		brd->ctn_steps = 0;
		if(max_steps > 0){
			brd->steps = (char*)malloc((sizeof(char) * max_steps) << 1);
			if(!brd->steps) return LLO_ERROR;
			memset(brd->steps, 0, (sizeof(char) * max_steps) << 1);
		}
		return LLO_OK;
	}
	return LLO_ERROR;
}

int board_result_copy(const board_result_ptr input, board_result_ptr output){
	if(!input)
		return LLO_ERROR;

	output = (board_result_ptr)malloc(sizeof(board_result_ptr));
	output->board_width = input->board_width;
	output->ctn_pins = input->ctn_pins;
	
	if(!output->begin_pins){
		output->begin_pins = (PIN*)malloc(sizeof(PIN)*input->ctn_pins);
		memcpy(output->begin_pins, input->begin_pins, sizeof(PIN)*input->ctn_pins);
	}
	if(!output->final_pins){
		output->final_pins = (PIN*)malloc(sizeof(PIN)*input->ctn_pins);
		memcpy(output->final_pins, input->final_pins, sizeof(PIN)*input->ctn_pins);
	}

	output->ctn_steps = input->ctn_steps;
	if(!input->steps){
		output->steps = (char*)malloc((sizeof(char) * input->ctn_steps) << 1);
		memcpy(output->steps, input->steps, (sizeof(char) * input->ctn_steps) << 1);
	}

	return LLO_OK;
}

void board_result_destory(board_result_ptr result){
	if(!result)
		return;

	if(result->begin_pins) { 
		free(result->begin_pins); 
		result->begin_pins = NULL; 
	}
	if(result->final_pins){ 
		free(result->final_pins); 
		result->final_pins = NULL; 
	}
	if(result->steps){ 
		free(result->steps); 
		result->steps = NULL; 
	}

	free(result);
	result = NULL;
}

int board_result_final_to_board(const board_result_ptr input, board_ptr output){
	if(input){
		output = (board_ptr)malloc(sizeof(board_ptr));
		output->board_width = input->board_width;
		output->ctn_pins = input->ctn_pins;
		if(input->final_pins){
			output->pins = (PIN*)malloc(sizeof(PIN) * input->ctn_pins);
			memcpy(output->pins, input->final_pins, sizeof(PIN) * input->ctn_pins);
		}
		return LLO_OK;
	}
	return LLO_ERROR;
}

int board_result_merge(board_result_ptr b0, board_result_ptr b1, board_result_ptr output){
	if(!b0 || !b1 || b0->board_width != b1->board_width || b0->ctn_pins != b1->ctn_pins)
		return LLO_ERROR;

	output = (board_result_ptr)malloc(sizeof(board_result_ptr));
	output->ctn_pins = b0->ctn_pins;
	output->board_width = b0->board_width;

	size_t board_size = sizeof(PIN) * output->ctn_pins;

	// copy the begin board from b0
	output->begin_pins = (PIN*)malloc(board_size);
	memcpy(output->begin_pins, b0->begin_pins, board_size);

	// copy the final board from b1
	output->final_pins = (PIN*)malloc(board_size);
	memcpy(output->final_pins, b1->final_pins, board_size);

	// concat the steps of b0 and b1
	size_t step_size = sizeof(char) << 1;
	output->ctn_steps = b0->ctn_steps + b1->ctn_steps;
	output->steps = (char*)malloc(step_size * output->ctn_steps);
	memcpy(output->steps, b0->steps, step_size * b0->ctn_steps);
	memcpy((void*)output->steps[step_size * b0->ctn_steps], (void*)b1->steps, step_size * b1->ctn_steps);

	return LLO_OK;
}

#include <math.h>
int board_max_ctn(int ctn_pins, int max_bfs){
	// max_bfs > 0: 1
	// others     : 1 + ctn_pins * (4^1 + 4^2 + 4^3 + 4^4 + 4^5 + 4^6 + 4^7....£©
	int result = 0;
	while(max_bfs > 0)
		result += pow(4.0, max_bfs--);
	result *= ctn_pins;
	return result + 1;
}

#endif // LLO_BOARD_CPP_