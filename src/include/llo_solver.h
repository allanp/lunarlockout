#ifndef LLO_SOLVER_H_
#define LLO_SOLVER_H_

#include "llo_internal.h"

int llo_solve(const board_ptr input, const int max_bfs, const int max_dfs, board_result_ptr *output, int &ctn_output);

int llo_solve_bfs(const board_ptr input, const int max_bfs, board_result_ptr *bfs_results, int &ctn_bfs_results);
int llo_solve_dfs(const board_ptr input, const int max_dfs, board_result_ptr *dfs_results, int &ctn_dfs_results);

#endif // LLO_SOLVER_H_