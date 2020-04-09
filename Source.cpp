#include <iostream>
#include "SudokuSolver.h"
using namespace std;

int main()
{
	SudokuSolver* currentSolver = new SudokuSolver;
	string path = "sudoku.txt";
	if (currentSolver->solveSudoku(path, 3) >= 1)
	{
		currentSolver->outputSudoku();
	}
	path = "sudoku2.txt";
	if (currentSolver->solveSudoku(path, 4) >= 1)
	{
		currentSolver->outputSudoku();
	}
	delete currentSolver;

	return 0;
}