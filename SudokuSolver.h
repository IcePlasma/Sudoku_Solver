#ifndef SUDOKU_SOLVER_H
#define SUDOKU_SOLVER_H
#include <string>
#include <iostream>
#include <fstream>
using namespace std;

//This is a simple brute force sudoku solver. I just made this for fun. Quite fast for aany given small sudoku. Not so fast for larger.
class SudokuSolver
{
private: 
	//Stores values that show the state of the sudoku's solution. Valid values are 0 (not solved), 
	//1 (solved), and -1 (unsolvable.)
	int solved;
	//Number of values in the entire sudoku.
	int size;
	//Number of values in a row or column of the sudoku.
	int base;
	//Number of values in a row or column of one square in the sudoku.
	int baseRoot;
	//Stores what row, index, and column each index of the sudoku board is in.
	int *rowIndexMap, *columnIndexMap, *squareIndexMap;
	//Stores what values have already been used in each row, column, and square of the sudoku
	int *rows, *columns, *squares;
	//Stores the current values for the sudoku being solved.
	int *sudokuBoard;
	const string pathNotFound = "File path not found.";
	const string unsolvable = "Puzzle is unsolvable.";

	//Populates the rowIndexMap, columnIndexMap, and squareIndexMap arrays.
	void mapIndexes();
	//Sets the dimensions of the sudoku and sets it up to be solved. This includes allocation of memory.
	void setDimension(int);
	//Resets the sudoku to an unsolved state.
	void resetSudoku();
	//Reads in the values for a sudoku found at the given file path.
	bool readInSudoku(string&);
	//Goes through each possible value that coulkd be inserted into the sudoku until it either runs out of 
	//values or solves the puzzle.
	void recursiveTest(int);
	//Allocates all of the dynamic memory used by this class.
	void reallocateDynamic();
	//Deallocates all of the currently allocate dynamic memory.
	void deallocateDynamic();

public:
	//Outputs and formats the current values in the sudoku.
	void outputSudoku();
	SudokuSolver();
	~SudokuSolver();
	//Solves a sudoku at the given file path. The int is the length and width of one of the sudoku's squares.
	bool solveSudoku(string&, int);
};

#endif
