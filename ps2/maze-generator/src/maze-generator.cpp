/**
 * File: maze-generator.cpp
 * ------------------------
 * Presents an adaptation of Kruskal's algorithm to generate mazes.
 */

#include <iostream>
using namespace std;

#include "console.h"
#include "simpio.h"
#include "maze-graphics.h"
#include "maze-types.h"
#include "set.h"
#include "vector.h"

void fillMainVectors (Vector<Set<cell>> & vectorOfChambers, Vector<wall> & walls, int dimension);
void drawGrid(MazeGeneratorView & mazeView, Vector<wall> & walls);
void shuffle(Vector<wall> & v);
int findCellInChamber(Vector<Set<cell>> v, cell c);
void buildMaze(Vector<wall> & walls, Vector<Set<cell>> & vectorOfChambers, MazeGeneratorView & mazeView);

static int getMazeDimension(string prompt, int minDimension = 7, int maxDimension = 50) {

	while (true) {
		int response = getInteger(prompt);
		if (response == 0) return response;
        if (response >= minDimension && response <= maxDimension) return response;
		cout << "Please enter a number between "
			 << minDimension << " and " 
		     << maxDimension << ", inclusive." << endl;
	}
}

void fillMainVectors (Vector<Set<cell>> & vectorOfChambers, Vector<wall> & walls, int dimension) {

    for (int i = 0; i < dimension; i++) {
        for (int j = 0; j < dimension; j++) {

            cell newCell;
            newCell.row = i;
            newCell.col = j;
            vectorOfChambers[dimension * i + j].add(newCell);
        }
    }

    for (int i = 0; i < dimension; i++) {

        for (int j = 0; (j < dimension) && ((i + 1) * (j + 1) < dimension * dimension); j++) {

            if ((i == dimension - 1) || (j == dimension - 1)) {

                cell newCell_1;
                cell newCell_2;

                newCell_1.row = i;
                newCell_1.col = j;

                newCell_2.row = i + ((j + 1) / dimension);
                newCell_2.col = j + ((i + 1) / dimension);

                wall newWall;
                newWall.one = newCell_1;
                newWall.two = newCell_2;
                walls[i * (2 * dimension - 1) + ((j + 1) / dimension + 1) * j] = newWall;
            }

            else {
                for (int k = 0; k < 2; k++) {

                    cell newCell_1;
                    cell newCell_2;

                    newCell_1.row = i;
                    newCell_1.col = j;

                    newCell_2.row = i + k;
                    newCell_2.col = j + (1 - k);

                    wall newWall;
                    newWall.one = newCell_1;
                    newWall.two = newCell_2;
                    walls[i * (2 * dimension - 1) + 2 * j + k] = newWall;
                }
            }
        }
    }

}

void drawGrid(MazeGeneratorView & mazeView, Vector<wall> & walls) {

    for (int i = 0; i < walls.size(); i++) {

        mazeView.drawWall(walls[i]);
    }
}

void shuffle(Vector<wall> & v) {
    for (int i = 0, length = v.size(); i < length; i++) {
        int j = randomInteger(i, length - 1);
        if (i != j) {
            wall temp = v[i];
            v[i] = v[j];
            v[j] = temp;
        }
    }
}

int findCellInChamber(Vector<Set<cell>> v, cell c) {

    for (int i = 0; i < v.size(); i++) {
        if (v[i].contains(c)) return i;
    }

    return -1;
}

void buildMaze(Vector<wall> & walls, Vector<Set<cell>> & vectorOfChambers, MazeGeneratorView & mazeView) {
    for (int i = 0; i < walls.size(); i++) {

        int i_1 = findCellInChamber(vectorOfChambers, walls[i].one);
        int i_2 = findCellInChamber(vectorOfChambers, walls[i].two);

        if (i_1 != i_2) {
            vectorOfChambers[i_1] += vectorOfChambers[i_2];
            vectorOfChambers[i_2].clear();
            mazeView.removeWall(walls[i]);
        }
    }
}

int main() {

    MazeGeneratorView mazeView;

	while (true) {
		int dimension = getMazeDimension("What should the dimension of your maze be [0 to exit]? ");
		if (dimension == 0) break;
		cout << "This is where I'd animate the construction of a maze of dimension " << dimension << "." << endl;

        mazeView.setDimension(dimension);
        mazeView.drawBorder();

        Vector<Set<cell>> vectorOfChambers(dimension * dimension);
        Vector<wall> walls(2 * dimension * (dimension - 1));

        fillMainVectors(vectorOfChambers, walls, dimension);

        drawGrid(mazeView, walls);

        shuffle(walls);

        buildMaze(walls,vectorOfChambers, mazeView);
	}

	return 0;
}
