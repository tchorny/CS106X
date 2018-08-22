/**
 * File: trees.cpp
 * ---------------
 * Draws a recursive tree as specified in the Assignment 3 handout.
 */

#include <string>    // for string
#include <iostream>  // for cout, endl
using namespace std;

#include "console.h" // required of all CS106 C++ programs
#include "gwindow.h" // for GWindow class and its setTitle, setColor, and drawPolarLine methods
#include "gtypes.h"  // for GPoint class
#include "gevents.h" // for the GMouseEvent type
#include "random.h"  // for randomChance function

const static double kWindowWidth = 600;
const static double kWindowHeight = 600;
const static string kWindowTitle = "Recursive Trees";
const static double kTrunkLength  = kWindowHeight/4;
const static double kShrinkFactor = 0.70;
const static int kBranchAngleSeparation = 15;
const static int kTrunkStartAngle = 90;
const static string kLeafColor = "#2e8b57";
const static string kTrunkColor = "#8b7765";
const static double kBranchProbability = 1.0;

static void drawTree(GWindow& window, int order, GPoint point, double length, double theta);
static void drawTree(GWindow& window, int order);

static void drawTree(GWindow& window, int order) {
    GPoint trunkBase(window.getWidth()/2, window.getHeight());
    GPoint pt;
    window.setColor(order < 2 ? kLeafColor : kTrunkColor);
    pt = window.drawPolarLine(trunkBase, kTrunkLength, kTrunkStartAngle);

    drawTree(window, order - 1, pt, kTrunkLength * kShrinkFactor, 90);

}



static void drawTree(GWindow& window, int order, GPoint point, double length, double theta) {

    if (order == 0) {
        for (int i; i < 7; i++) {
            window.drawPolarLine(point, length, theta - 45 + 15 * i);
            window.setColor(kLeafColor);
        }
        return;
    }

    drawTree(window, order - 1, window.drawPolarLine(point, length, theta + 45), kShrinkFactor * length, theta + 45);
    drawTree(window, order - 1, window.drawPolarLine(point, length, theta + 30), kShrinkFactor * length, theta + 30);
    drawTree(window, order - 1, window.drawPolarLine(point, length, theta + 15), kShrinkFactor * length, theta + 15);
    drawTree(window, order - 1, window.drawPolarLine(point, length, theta), kShrinkFactor * length, theta);
    drawTree(window, order - 1, window.drawPolarLine(point, length, theta - 15), kShrinkFactor * length, theta - 15);
    drawTree(window, order - 1, window.drawPolarLine(point, length, theta - 30), kShrinkFactor * length, theta - 30);
    drawTree(window, order - 1, window.drawPolarLine(point, length, theta - 45), kShrinkFactor * length, theta - 45);
}


const static int kHighestOrder = 5;
int main() {

    GWindow window(kWindowWidth, kWindowHeight);
    window.setWindowTitle(kWindowTitle);

    drawTree(window, 6);


    return 0;
}
