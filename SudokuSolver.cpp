#include "SudokuSolver.h"
#define WINDOW_WIDTH 237

SudokuSolver::SudokuSolver()
{
	squareIndexMap = NULL;
	rowIndexMap = NULL;
	columnIndexMap = NULL;
	rows = NULL;
	columns = NULL;
	squares = NULL;
	sudokuBoard = NULL;
	size = 0;
	base = 0;
	baseRoot = 0;
}

SudokuSolver::~SudokuSolver()
{
	deallocateDynamic();
}

//Deallocates all of the currently allocate dynamic memory.
void SudokuSolver::deallocateDynamic()
{
	if (sudokuBoard != NULL)
	{
		delete[] sudokuBoard;
		sudokuBoard = NULL;
	}
	if (squareIndexMap != NULL)
	{
		delete[] squareIndexMap;
		squareIndexMap = NULL;
	}
	if (rowIndexMap != NULL)
	{
		delete[] rowIndexMap;
		rowIndexMap = NULL;
	}
	if (columnIndexMap != NULL)
	{
		delete[] columnIndexMap;
		columnIndexMap = NULL;
	}
	if (rows != NULL)
	{
		delete[] rows;
		rows = NULL;
	}
	if (columns != NULL)
	{
		delete[] columns;
		columns = NULL;
	}
	if (squares != NULL)
	{
		delete[] squares;
		squares = NULL;
	}
}

//Allocates all of the dynamic memory used by this class.
void SudokuSolver::reallocateDynamic()
{
	if (baseRoot > 1)
	{
		if (sudokuBoard == NULL)
		{
			sudokuBoard = new int[size];
		}
		if (squareIndexMap == NULL)
		{
			squareIndexMap = new int[size];
		}
		if (rowIndexMap == NULL)
		{
			rowIndexMap = new int[size];
		}
		if (columnIndexMap == NULL)
		{
			columnIndexMap = new int[size];
		}
		if (rows == NULL)
		{
			rows = new int[base];
		}
		if (columns == NULL)
		{
			columns = new int[base];
		}
		if (squares == NULL)
		{
			squares = new int[base];
		}
	}
}

//Populates the rowIndexMap, columnIndexMap, and squareIndexMap arrays.
void SudokuSolver::mapIndexes()
{
	for (int i = 0; i < size; i++)
	{
		rowIndexMap[i] = i / base;
		columnIndexMap[i] = i % base;
		squareIndexMap[i] = (rowIndexMap[i] / baseRoot) + (columnIndexMap[i] / baseRoot) * baseRoot;
	}
}

//Sets the dimensions of the sudoku and sets it up to be solved. This includes allocation of memory.
void SudokuSolver::setDimension(int newDimension)
{
	if (newDimension != baseRoot && newDimension > 1)
	{
		baseRoot = newDimension;
		base = newDimension * newDimension;
		size = base * base;
		deallocateDynamic();
		reallocateDynamic();
		resetSudoku();
		mapIndexes();
	}
}

//Resets the sudoku to an unsolved state.
void SudokuSolver::resetSudoku()
{
	if (rows != NULL && columns != NULL && squares != NULL)
	{
		for (int i = 0; i < base; i++)
		{
			rows[i] = 0;
			columns[i] = 0;
			squares[i] = 0;
		}
	}
	solved = 0;
}

//Reads in the initial values for a sudoku. Format for being read in is value[white space character]value[white space character]...value
//Values are inserted into the top left position first and then goes left to right in each row.
//Returns false if the file is unable to be read or the sudoku is unsolvable.
bool SudokuSolver::readInSudoku(string& filePath)
{
	if (baseRoot > 1)
	{
		ifstream sudokuFile;
		sudokuFile.open(filePath);
		if (!sudokuFile.is_open())
		{
			cout << pathNotFound << endl;
			return false;
		}
		int i;
		for (i = 0; i < size; i++)
		{
			if (!(sudokuFile >> sudokuBoard[i]))
			{
				break;
			}
			if (sudokuBoard[i] <= 0 || sudokuBoard[i] > base)
			{
				sudokuBoard[i] = 0;
			}
			sudokuBoard[i]--;

			if (sudokuBoard[i] != -1)
			{
				if (!((rows[rowIndexMap[i]] | columns[columnIndexMap[i]] | squares[squareIndexMap[i]]) & (1 << sudokuBoard[i])))
				{
					rows[rowIndexMap[i]] |= (1 << sudokuBoard[i]);
					columns[columnIndexMap[i]] |= (1 << sudokuBoard[i]);
					squares[squareIndexMap[i]] |= (1 << sudokuBoard[i]);
				}
				else
				{
					solved = -1;
					return false;
				}
			}
		}
		sudokuFile.close();
		for (;i < size; i++)
		{
			sudokuBoard[i] = -1;
		}
		return true;
	}
	return false;
}

//Goes through each possible value that coulkd be inserted into the sudoku until it either runs out of 
//values or solves the puzzle.
void SudokuSolver::recursiveTest(int index)
{
	while (index < size && sudokuBoard[index] != -1)
	{
		index++;
	}
	if (index < size)
	{
		int value = 0;
		int impossibleValues = rows[rowIndexMap[index]] | columns[columnIndexMap[index]] | squares[squareIndexMap[index]];
		while (value < base && !solved)
		{
			while ((impossibleValues & (1 << value)) && value < base)
			{
				value++;
			}
			if (!(impossibleValues & (1 << value)) && value < base)
			{
				rows[rowIndexMap[index]] |= (1 << value);
				columns[columnIndexMap[index]] |= (1 << value);
				squares[squareIndexMap[index]] |= (1 << value);
				sudokuBoard[index] = value;
				if (index < size - 1)
				{
					recursiveTest(index + 1);
					if (!solved)
					{
						rows[rowIndexMap[index]] ^= (1 << value);
						columns[columnIndexMap[index]] ^= (1 << value);
						squares[squareIndexMap[index]] ^= (1 << value);
						sudokuBoard[index] = -1;
						value++;
					}
				}
				else
				{
					solved = 1;
				}
			}
		}
	}
	else
	{
		solved = 1;
	}
}

//Outputs and formats the current values in the sudoku.
void SudokuSolver::outputSudoku()
{
	if (sudokuBoard != NULL)
	{
		for (int i = 0; i < base; i++)
		{
			if (i != 0)
			{
				cout << endl;
			}
			if (i % baseRoot == 0)
			{
				for (int i = 0; i < WINDOW_WIDTH; i++)
				{
					cout << "-";
				}
				cout << endl;
			}
			for (int j = 0; j < base; j++)
			{
				if (j % baseRoot == 0)
				{
					cout << "| ";
				}
				cout << sudokuBoard[i * base + j] + 1;
				if (base > 10 && sudokuBoard[i * base + j] < 10)
				{
					cout << "  ";
				}
				else
				{
					cout << ' ';
				}
			}
			cout << '|';
			cout << endl;
		}
		for (int i = 0; i < WINDOW_WIDTH; i++)
		{
			cout << "-";
		}
		cout << endl;
	}
}

//Solves a sudoku at the given file path. The int is the length and width of one of the sudoku's squares.
bool SudokuSolver::solveSudoku(string& filePath, int baseRootValue)
{
	setDimension(baseRootValue);
	if (!readInSudoku(filePath))
	{
		if (solved < 0)
		{
			cout << unsolvable << endl;
		}
		return false;
	}
	recursiveTest(0);
	return solved;
}
