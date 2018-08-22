/**
 * File: life.cpp
 * --------------
 * Implements the Game of Life.
 */

#include <iostream>  // for cout
#include <string>
#include <sstream>
#include <stdint.h>
using namespace std;

#include "console.h" // required of all files that contain the main function
#include "simpio.h"  // for getLine
#include "gevents.h" // for mouse event detection
#include "strlib.h"
#include "grid.h"
#include "random.h"

#include "life-constants.h"  // for kMaxAge
#include "life-graphics.h"   // for class LifeDisplay

// Prototypes
static void welcome();
void promtUserForFile(ifstream & infile, string prompt);
void parseIntegers(ifstream & infile, int & rows, int & cols);
void parseGrid(ifstream & infile, Grid<uint8_t> & grid_0);
void gridTransformation(Grid<uint8_t> & grid_0, Grid<uint8_t> & grid_1);
void graphGrid(Grid<uint8_t> & grid, LifeDisplay & display);
void fillRandomGrid(Grid<uint8_t> & grid);
void simulation(Grid<uint8_t> & grid_0, Grid<uint8_t> & grid_1, LifeDisplay & display, bool isManual);
void buildInitialGrid(Grid<uint8_t> & grid_0, Grid<uint8_t> & grid_1, LifeDisplay & display, bool isRandom);
void waitForPrompt();
void waitForMouseClick();
bool isTrue(string question);
bool isAgeStable(Grid<uint8_t> & grid);
bool isGridStable(Grid<uint8_t> & grid_0, Grid<uint8_t> & grid_1);
int gameSpeed();
int randomGridSize();

int main() {

    LifeDisplay display;
    display.setTitle("Game of Life");

    while (true) {

    Grid<uint8_t> grid_0;
    Grid<uint8_t> grid_1;

    welcome();

    bool isManual = isTrue("Do you want to use Manual mode?");
    bool isRandom = isTrue("Do you want to use Random grid?");

    buildInitialGrid(grid_0, grid_1, display, isRandom);

    graphGrid(grid_0, display);
    gridTransformation(grid_0, grid_1);

    waitForMouseClick();

    simulation(grid_0, grid_1, display, isManual);

    if (!isTrue("Do you want to play again?")) break;

    }

    return 0;
}

/* Print out greeting for beginning of program. */
static void welcome() {
    cout << "Welcome to the game of Life, a simulation of the lifecycle of a bacteria colony." << endl;
    cout << "Cells live and die by the following rules:" << endl << endl;
    cout << "\tA cell with 1 or fewer neighbors dies of loneliness" << endl;
    cout << "\tLocations with 2 neighbors remain stable" << endl;
    cout << "\tLocations with 3 neighbors will spontaneously create life" << endl;
    cout << "\tLocations with 4 or more neighbors die of overcrowding" << endl << endl;
    cout << "In the animation, new cells are dark and fade to gray as they age." << endl << endl;
    getLine("Hit [enter] to continue....   ");
    cout << "You can start your colony with random cells or read from a prepared file." << endl;
}

void promtUserForFile(ifstream & infile, string prompt) {
    while (true) {
        cout << prompt;
        string filename;
        cin.sync();
        cin.clear();
        getline (cin, filename);
        infile.open(filename.c_str());
        if (!infile.fail()) break;
        infile.clear();
        cout << "Unable to open file. Try again." << endl;
        if (prompt == "") prompt = "Input file: ";
    }
}

void parseIntegers(ifstream & infile, int & rows, int & cols) {
    string line;
    int dim;
    bool isRowsDone = false;

    while (getline (infile, line)) {
        istringstream strstream (line);
        strstream >> dim >> ws;

        if (!strstream.fail() && isRowsDone == false) {
            rows = dim;
            isRowsDone = true;
        }

        else if (!strstream.fail() && isRowsDone == true) {
            cols = dim;
            break;
        }
    }
}

void parseGrid(ifstream & infile, Grid<uint8_t> & grid_0) {
    char ch;
    int i = 0;
    const int nRows = grid_0.numRows();
    const int nCols = grid_0.numCols();
    while (infile.get(ch)) {

        if (ch == 'x') {
            grid_0[i / nRows][i % nRows] = 1;
            i++;
        }
        else if (ch == '-') {
            grid_0[i / nRows][i % nRows] = 0;
            i++;
        }
        if (i >= nRows * nCols) break;
    }
}

void gridTransformation(Grid<uint8_t> & grid_0, Grid<uint8_t> & grid_1) {
    const int nRows = grid_0.numRows();
    const int nCols = grid_0.numCols();

    for (int i = 0; i < nRows; i++) {
        for (int j = 0; j < nCols; j++) {

            int lives = 0;

            for (int m = -1; m <= 1; m ++) {
                for(int n = -1; n <= 1; n ++) {
                    if ((m != 0 || n != 0) && grid_0.inBounds(i + m, j + n) && (grid_0[i + m][j + n] > 0)) lives++;
                }
            }

            if (lives <= 1 || lives >= 4) grid_1[i][j] = 0;
            else if (lives == 2) {
                if (grid_0[i][j] == 0) grid_1[i][j] = 0;
                else grid_1[i][j] = ((grid_0[i][j] == 12) ? 12 : grid_0[i][j] + 1);
            }
            else if (lives == 3) grid_1[i][j] = ((grid_0[i][j] == 12) ? 12 : grid_0[i][j] + 1);

        }

    }
}

void graphGrid(Grid<uint8_t> & grid, LifeDisplay & display) {

    const int nRows = grid.numRows();
    const int nCols = grid.numCols();

    for (int i = 0; i < nRows; i++) {
        for (int j = 0; j < nCols; j++) {

            display.drawCellAt(i, j, grid[i][j]);

        }
    }

}

void fillRandomGrid(Grid<uint8_t> & grid) {

    const int nRows = grid.numRows();
    const int nCols = grid.numCols();

    for (int i = 0; i < nRows; i++) {
        for (int j = 0; j < nCols; j++) {

            grid[i][j] = randomInteger(0, 1);

        }
    }
}

bool isGridStable(Grid<uint8_t> & grid_0, Grid<uint8_t> & grid_1) {

    const int nRows = grid_0.numRows();
    const int nCols = grid_0.numCols();

    for (int i = 0; i < nRows; i++) {
        for (int j = 0; j < nCols; j++) {

            if ((grid_0[i][j] == 0 || grid_1[i][j] == 0) && (grid_0[i][j] != grid_1[i][j])) return false;
        }
    }
    cout << "isGrid error" << endl;
    return true;
}

bool isAgeStable(Grid<uint8_t> & grid) {

    const int nRows = grid.numRows();
    const int nCols = grid.numCols();

    for (int i = 0; i < nRows; i++) {
        for (int j = 0; j < nCols; j++) {

            if (grid[i][j] > 0 && grid[i][j] < kMaxAge) return false;
        }
    }
    cout << "isAge error" << endl;
    return true;
}


bool isTrue(string question) {

    string input;

    cout << question << " Yes/Y" << endl;
    cin >> input;

    if (toUpperCase(input) == "Y" || toUpperCase(input) == "YES") return true;
    else return false;

}

int gameSpeed() {

    while (true) {

        string input;
        int speed;

        cout << "Please choose game speed from 1 to 3" << endl;
        cin >> input;
        istringstream strstream(input);
        strstream >> speed >> ws;

        if (!strstream.fail()) {

            switch (speed) {
            case 1: case 2: case 3:
                return speed;
                break;
            default:
                cout << "Wrong number" << endl;
                break;
            }
        }
        else cout << "Not numerical" << endl;

    }
}


int randomGridSize() {

    while (true) {

        string input;
        int randGridSize;

        cout << "Please choose random grid size, integer from 3" << endl;
        cin >> input;
        istringstream strstream(input);
        strstream >> randGridSize >> ws;

        if (!strstream.fail()) {

            if (randGridSize >= 3) return randGridSize;
            else cout << "Wrong number" << endl;

        }

        else cout << "Not numerical" << endl;

    }
}


void simulation(Grid<uint8_t> & grid_0, Grid<uint8_t> & grid_1, LifeDisplay & display, bool isManual) {

    short speed = 0;

    if (!isManual) speed = baseSpeed * (3 - gameSpeed());

    while (!isGridStable(grid_0, grid_1) && !isAgeStable(grid_1) && (getNextEvent(CLICK_EVENT).getEventType() == NULL_EVENT)) {

        grid_0 = grid_1;
        graphGrid(grid_0, display);
        gridTransformation(grid_0, grid_1);

        if (isManual) {
            waitForPrompt();
        }
        else pause(speed);

    }
}

void waitForPrompt() {

    string toContinue;
    cout << "Press 'Enter' to continue" << endl;

    do {
        cin.clear();
        cin.sync();
        getline (cin, toContinue);
    } while (cin.fail() || (toContinue != ""));


}

void buildInitialGrid(Grid<uint8_t> & grid_0, Grid<uint8_t> & grid_1, LifeDisplay & display, bool isRandom) {

    switch (isRandom) {

        case true:
        {
            int randGridSize = randomGridSize();
            display.setDimensions(randGridSize, randGridSize);
            grid_0.resize(randGridSize, randGridSize);
            grid_1.resize(randGridSize, randGridSize);
            fillRandomGrid(grid_0);
            break;
        }
        case false:
        {
            ifstream infile;
            int rows, cols;
            promtUserForFile(infile, "Input file: ");
            parseIntegers(infile, rows, cols);
            display.setDimensions(rows, cols);
            grid_0.resize(rows, cols);
            grid_1.resize(rows, cols);
            parseGrid(infile, grid_0);
            infile.close();
            break;
        }
    }
}

void waitForMouseClick() {

    cout << "Click mouse to continue" << endl;

    while (true) {
        GMouseEvent click = waitForClick();
        if (click.getEventType() != NULL_EVENT) break;
    }

}
