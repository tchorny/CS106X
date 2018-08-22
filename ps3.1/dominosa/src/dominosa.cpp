/**
 * File: dominosa.cpp
 * ------------------
 * This animates the brute-force discovery of a solution
 * to a 2 x n dominosa board.
 */

#include <iostream>
#include <cmath>
using namespace std;

#include "simpio.h"
#include "grid.h"
#include "random.h"
#include "dominosa-graphics.h"
#include "vector.h"

static bool canSolveBoard(DominosaDisplay& display, Grid<int>& board, Vector<Vector<int>> & used, int col);
bool doesVectorContainVector(Vector<Vector<int>> & vec, Vector<int> & value);

bool doesVectorContainVector(Vector<Vector<int>> & vec, Vector<int> & value) {

    if (vec.isEmpty()) return false;
    for (int i = 0; i < vec.size(); i++) {
        if (vec[i] == value) return true;
    }
    return false;
}

static void welcome() {
	cout << "Here we'll illustrate the use of recursive backtracking to" << endl;
	cout << "discover a solution to various 2 x n Dominosa boards.  In some" << endl;
	cout << "cases there won't be any solutions, and in the cases where there are" << endl;
	cout << "multiple solutions, we'll just find one of them." << endl;
	cout << endl;
}

static int getIntegerInRange(string prompt, int low, int high) {
	while (true) {
		int response = getInteger(prompt);
		if (response == 0 || (response >= low && response <= high)) return response;
		cout << "Sorry, but I can't use that number." << endl;
	}
}

static void populateBoard(Grid<int>& board, int low, int high) {
	for (int row = 0; row < board.numRows(); row++) {
		for (int col = 0; col < board.numCols(); col++) {
			board[row][col] = randomInteger(low, high);
		}
	}
}

static bool canSolveBoard(DominosaDisplay& display, Grid<int>& board, Vector<Vector<int>> & used, int col) {

    Vector<int> newpair(2), yetnewpair(2);

    coord first, second;

    if (board.numCols() < col) return true;

    if (board.numCols() == col) {
        newpair[0] = board[0][col - 1] < board[1][col - 1] ? board[0][col - 1] : board[1][col - 1];
        newpair[1] = board[0][col - 1] < board[1][col - 1] ? board[1][col - 1] : board[0][col - 1];
        if (doesVectorContainVector(used, newpair)) return false;
        else {
            first.row = 0;
            first.col = col - 1;
            second.row = 1;
            second.col = col - 1;
            display.provisonallyPair(first, second);
            return true;
        }
    }

    newpair[0] = board[0][col - 1] < board[1][col - 1] ? board[0][col - 1] : board[1][col - 1];
    newpair[1] = board[0][col - 1] < board[1][col - 1] ? board[1][col - 1] : board[0][col - 1];

    if (!doesVectorContainVector(used, newpair)) {

        first.row = 0;
        first.col = col - 1;
        second.row = 1;
        second.col = col - 1;
        display.provisonallyPair(first, second);

        used.add(newpair);
        if (canSolveBoard(display, board, used, col + 1)) return true;
        else {
            used.remove(used.size() - 1);
            display.eraseProvisionalPairing(first, second);
        }
    }

    newpair[0] = board[0][col - 1] < board[0][col] ? board[0][col - 1] : board[0][col];
    newpair[1] = board[0][col - 1] < board[0][col] ? board[0][col] : board[0][col - 1];

    if (!doesVectorContainVector(used, newpair)) {
        used.add(newpair);

        first.row = 0;
        first.col = col - 1;
        second.row = 0;
        second.col = col;
        display.provisonallyPair(first, second);

        yetnewpair[0] = board[1][col - 1] < board[1][col] ? board[1][col - 1] : board[1][col];
        yetnewpair[1] = board[1][col - 1] < board[1][col] ? board[1][col] : board[1][col - 1];

        if (!doesVectorContainVector(used, yetnewpair)) {
            used.add(yetnewpair);

            first.row = 1;
            first.col = col - 1;
            second.row = 1;
            second.col = col;
            display.provisonallyPair(first, second);

            if (canSolveBoard(display, board, used, col + 2)) return true;
            else used.remove(used.size() - 1);

            display.eraseProvisionalPairing(first, second);
        }

        used.remove(used.size() - 1);

        first.row = 0;
        first.col = col - 1;
        second.row = 0;
        second.col = col;
        display.eraseProvisionalPairing(first, second);

    }
    return false;
}

static bool canSolveBoard(DominosaDisplay& display, Grid<int>& board) {
    Vector<Vector<int>> used;
    return canSolveBoard(display, board, used, 1);
}

int main() {
	DominosaDisplay display;
	welcome();
	while (true) {
		int numColumns = getIntegerInRange("How many columns? [0 to exit]: ", 9, 25);
		if (numColumns == 0) break;
		Grid<int> board(2, numColumns);
		populateBoard(board, 1, ceil(2 * sqrt(numColumns)));
		display.drawBoard(board);
        if (canSolveBoard(display, board)) {
			cout << "The board can be solved, and one such solution is drawn above." << endl;
		} else {
			cout << "This board you see can't be solved." << endl;
		}
	}
	
	cout << "Okay, thanks for watching, and come back soon." << endl;
    cout << "Click the mouse anywhere in the window to exit." << endl;
	return 0;
}
