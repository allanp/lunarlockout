#ifndef LLO_BOARD_H_
#define LLO_BOARD_H_

#include <stdio.h>

int __ctn_pins = 0;
int __board_width = 0;

int validate_board_setting(int ctn_pins, int board_width){
	return ctn_pins > 0 && board_width > 2 && 
		   ctn_pins < board_width * board_width;
}

void set_board(int ctn_pins, int board_width){

	if(!validate_board_setting(ctn_pins, board_width)){
		fprintf(stderr, "invalid board setting.\n");
		exit(-1);
	}
	__ctn_pins = ctn_pins;
	__board_width = board_width;
}

inline const int get_ctn_pins() { return NUM_PINS; } // __ctn_pins; }
void set_num_pins(int ctn_pins){ set_board(ctn_pins, __board_width); }

void set_board_width(int board_width){ set_board(__ctn_pins, board_width); }
inline const int get_board_width() { return BOARD_WIDTH; } // __board_width; }

void create_boards_filename(int bmax, int npin, char* filename){
  if(!filename){
    fprintf(stderr, "uninitialized filename.\n");
    exit(-1);
  }
  sprintf(filename, "b%d_p%d.txt", BOARD_MAX + 1, NUM_PINS); // bmax, npin);

  if(!filename){
    fprintf(stderr, "failed to create filename.\n");
    exit(-1);
  }
}

void create_boards_file(const char* filename, bool ov){
  FILE *f = fopen(filename, ov ? "w" : "a");
  if(!f){
    fprintf(stderr, "Cannot write to file %s\n", filename);
    exit(-1);
  }
  fclose(f);
}

void save_board_to_file(int* board, int npin, const char* filename){
	
  FILE* output = NULL;

  if(filename == NULL) {
    output = stdout;
  }
  else{
    output = fopen(filename, "a");
    if(!output){
      fprintf(stderr, "Cannot write to file %s\n", filename);
      exit(-1);
    }
  }

  fprintf(output, "%d", board[0]);
  for(int i = 1; i<npin; i++)
    fprintf(output, " %d", board[i]);
  fprintf(output, "\n");
  
  if(filename) 
    fclose(output);
}

void gen_boards_to_file(int bmax, int npin, const char* filename){
  
	if(bmax < npin) {
		perror("Either the board is too small, or there are too many pins.");
		exit(-1);
	}
	
	int *board = (int*)malloc(sizeof(int)*npin);
	memset(board, 0, sizeof(int)*npin);

	int p0 = 0; // p' position, 0 ~ bmax-1
	int bn = bmax - npin;

	while(p0 < bmax){
		int start_p = (p0 != 0) ? 0 : p0;  // check board[1]'s position from
		memset(board, 0, sizeof(int)*npin); // clear the board
		board[0] = p0; // place the p'
    
		while(true){
			if( (p0 < bn && board[1] > bn)  ||
				(p0 == bn && board[1] > bn) ||
				(p0 > bn && board[1] >= bn) )
				break;
      
			int delta = 0;
			// place the board[1 ~ 'npin-2'] pins
			for(int p = 1; p < npin - 1; p++){
				if( start_p + delta == board[0] ) 
					delta++; // skips the p' position
				board[p] = start_p + delta++;
			}
			// start_p = board[1];
			board[npin-1] = board[npin-2]; // place the board[npin-1] pin
			//if(board[npin-1] > bmax - 1) 
			//  break;
			do{
				board[npin-1]++;   // increment the last p
				if(board[npin-1] == p0)
					board[npin-1]++;

				save_board_to_file(board, npin, filename); // found a new combination
				// check last pin's "board[bmax-1]" position 
				if(board[npin-1] >= (board[0] == bmax - 1 ? bmax - 2 : bmax - 1)){
					start_p = board[1];
					break;
				}
			} while(true);
			start_p++;
		}
		p0++;
	}
  printf("all boards have saved.\n");

}

void gen_all_boards(char* filename){
	if(!filename){
		fprintf(stderr, "invalid filename.\n");
		exit(-1);
	}

	const int max_board = get_board_width() * get_board_width();

	create_boards_filename(max_board, get_ctn_pins(), filename);
	create_boards_file(filename, true);
	gen_boards_to_file(max_board, get_ctn_pins(), filename);
}

#endif // LLO_BOARD_H_
