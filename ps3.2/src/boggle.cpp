/**
 * File: boggle.cpp
 * ----------------
 * Implements the game of Boggle.
 */
 
#include <cctype>
#include <iostream>
#include <string>

using namespace std;

#define BIG 5
#define SMALL 4

#include "simpio.h"
#include "grid.h"
#include "gwindow.h"
#include "gboggle.h"
#include "random.h"
#include "lexicon.h"

const int kBoggleWindowWidth = 650;
const int kBoggleWindowHeight = 350;

static void showLetters(const Grid<char> & board);
static void grandShuffle(Grid<string> & board);
static void toUpperCaseInPlace(string & str);
static bool isWordOnBoard(Grid<char> board, const string word);
static bool isWordOnBoard(Grid<char> & board, const string word, const uint8_t row, const uint8_t col);
static Grid<char> fillGrid();
static Lexicon humanTurn(const Lexicon & english, const Grid<char> & board);
static void computerTurn(const Lexicon & english, Grid<char> & board, const Lexicon & humanWords, Lexicon & computerWords,
                     const uint8_t row, const uint8_t col, string word);
static void computerTurn(const Lexicon & english, Grid<char> board, const Lexicon & humanWords);
static string toUpperStr(const string str);

const string kStandardCubes[16] = {
   "AAEEGN", "ABBJOO", "ACHOPS", "AFFKPS",
   "AOOTTW", "CIMOTU", "DEILRX", "DELRVY",
   "DISTTY", "EEGHNW", "EEINSU", "EHRTVW",
   "EIOSST", "ELRTTY", "HIMNQU", "HLNNRZ"
};

const string kBigBoggleCubes[25] = {
   "AAAFRS", "AAEEEE", "AAFIRS", "ADENNN", "AEEEEM",
   "AEEGMU", "AEGMNN", "AFIRSY", "BJKQXZ", "CCNSTW",
   "CEIILT", "CEILPT", "CEIPST", "DDLNOR", "DDHNOT",
   "DHHLOR", "DHLNOR", "EIIITT", "EMOTTT", "ENSSSU",
   "FIPRSY", "GORRVW", "HIPRRY", "NOOTUW", "OOOTTU"
};

static void welcome() {
    cout << "Welcome!  You're about to play an intense game ";
    cout << "of mind-numbing Boggle.  The good news is that ";
    cout << "you might improve your vocabulary a bit.  The ";
    cout << "bad news is that you're probably going to lose ";
    cout << "miserably to this little dictionary-toting hunk ";
    cout << "of silicon.  If only YOU had a gig of RAM..." << endl << endl;
}

static void giveInstructions() {
    cout << endl;
    cout << "The boggle board is a grid onto which I ";
    cout << "I will randomly distribute cubes. These ";
    cout << "6-sided cubes have letters rather than ";
    cout << "numbers on the faces, creating a grid of ";
    cout << "letters on which you try to form words. ";
    cout << "You go first, entering all the words you can ";
    cout << "find that are formed by tracing adjoining ";
    cout << "letters. Two letters adjoin if they are next ";
    cout << "to each other horizontally, vertically, or ";
    cout << "diagonally. A letter can only be used once ";
    cout << "in each word. Words must be at least four ";
    cout << "letters long and can be counted only once. ";
    cout << "You score points based on word length: a ";
    cout << "4-letter word is worth 1 point, 5-letters ";
    cout << "earn 2 points, and so on. After your puny ";
    cout << "brain is exhausted, I, the supercomputer, ";
    cout << "will find all the remaining words and double ";
    cout << "or triple your paltry score." << endl << endl;
    cout << "Hit return when you're ready...";
    getLine();
}

static string toUpperStr(const string str) {

    string result = "";
    for (uint8_t i = 0; i < str.length(); i++) result += toupper(str[i]);
    return result;
}

static void toUpperCaseInPlace(string & str) {
    for (uint8_t i = 0; i < str.length(); i++) str[i] = toupper(str[i]);
}

static Grid<char> fillGrid() {

    const bool isBig = getYesOrNo("You can choose standard Boggle "
        "(4x4 grid) or Big Boggle (5x5). Would you like Big Boggle?");

    const uint8_t size = isBig ? BIG : SMALL;

    Grid<string> board(size, size);

    const bool isCustomBoard = getYesOrNo("I'll give you a chance "
        "to set up the board to your specification, which makes it easier "
        "to confirm your boggle program is working. Do you want to force the board configuration?");

    if (isCustomBoard) {

        cout << "Enter a 25-character string to identify which letters you want on the cubes."
                "The first 5 letters are the cubes on the top row from left to right, the next "
                "5 letters are the second row, and so on." << endl;

        string input;

        do {
            input.clear();
            cin.clear();
            cout << "Enter the string: ";
            getline(cin, input);

        } while (input.length() < size * size * 6);

        for (uint8_t i = 0; i < size; i++) {
            for (uint8_t j = 0; j < size; j++) {
                board[i][j] = toUpperStr(input.substr((i * size + j) * 6, 6));
            }
        }
    }
    else {
        if (size == BIG) {
            for (uint8_t i = 0; i < size; i++) {
                for (uint8_t j = 0; j < size; j++) {
                    board[i][j] = kBigBoggleCubes[5 * i + j];
                }
            }
        }
        else {
            for (uint8_t i = 0; i < size; i++) {
                for (uint8_t j = 0; j < size; j++) {
                    board[i][j] = kStandardCubes[4 * i + j];
                }
            }
        }
    }

    grandShuffle(board);

    Grid<char> box(size, size);

    for (uint8_t i = 0; i < size; i++) {
        for (uint8_t j = 0; j < size; j++) {
            box[i][j] = board[i][j][0];
        }
    }

    return box;
}

static void showLetters(const Grid<char> & board) {

    const uint8_t size = board.numCols();
    for (uint8_t i = 0; i < size; i++) {
        for (uint8_t j = 0; j < size; j++) {
            labelCube (i, j, board[i][j]);
        }
    }
}

static void grandShuffle(Grid<string> & board) {

    const uint8_t size = board.numCols();

    for (uint8_t i = 0; i < size; i++) {
        for (uint8_t j = 0; j < size; j++) {
            if (i * size + j + 2 <= size * size) {
                uint8_t ran = randomInteger(i * size + j + 2, size * size);
                uint8_t row = (ran - 1) / size;
                uint8_t col = (ran - 1) % size;
                for (uint8_t k = 0; k < 6; k++) {
                    char temp = board[i][j][k];
                    board[i][j][k] = board[row][col][k];
                    board[row][col][k] = temp;
                }
            }
        }
    }

    for (uint8_t i = 0; i < size; i++) {
        for (uint8_t j = 0; j < size; j++) {
            uint8_t ran = randomInteger(0, 5);
            if (ran != 0) {
                char temp = board[i][j][0];
                board[i][j][0] = board[i][j][ran];
                board[i][j][ran] = temp;
            }
        }
    }
}

static Lexicon humanTurn(const Lexicon & english, const Grid<char> & board) {

    Lexicon humanWords;
    string input;

    do {
        input.clear();
        cin.clear();

        cout << "Enter a word: ";
        getline(cin, input);

        toUpperCaseInPlace(input);

        if (input.length() > 3) {

            if(!humanWords.contains(input)) {

                if(english.contains(input)) {

                    if (isWordOnBoard(board, input)) {

                        humanWords.add(input);
                        recordWordForPlayer(input, HUMAN);
                    }

                }
                else cout << "That's not a word!" << endl;
            }
            else cout << "Word already used." << endl;
        }
        else if (input != "") cout << "Too short word." << endl;

    } while (input != "");

    return humanWords;
}

static void computerTurn(const Lexicon & english, Grid<char> & board, const Lexicon & humanWords, Lexicon & computerWords,
                     const uint8_t row, const uint8_t col, string word) {

    const uint8_t len = word.length();

    for (int8_t i = -1; i <= 1; i++) {
        for (int8_t j = -1; j <= 1; j++) {

            if (board.inBounds(row + i, col + j)) {
                if ('*' != board[row + i][col + j]) {

                    string newstr = word + board[row + i][col + j];
                    if (english.containsPrefix(newstr)) {

                        if (len > 2 && (!computerWords.contains(newstr)) && (!humanWords.contains(newstr))
                                && english.contains(newstr)) {
                            computerWords.add(newstr);
                            recordWordForPlayer(newstr, COMPUTER);
                        }
                        board[row + i][col + j] = '*';
                        computerTurn(english, board, humanWords, computerWords, row + i, col + j, newstr);
                        board[row + i][col + j] = newstr[newstr.length() - 1];
                    }
                }
            }
        }
    }
}

static void computerTurn(const Lexicon & english, Grid<char> board, const Lexicon & humanWords) {

    const uint8_t size = board.numCols();
    Lexicon computerWords;
    
    for (uint8_t i = 0; i < size; i++) {
        for (uint8_t j = 0; j < size; j++) {

            string newstr = string(1, board[i][j]);
            if (english.containsPrefix(newstr)) {

                board[i][j] = '*';
                computerTurn(english, board, humanWords, computerWords, i, j, newstr);
                board[i][j] = newstr[0];
            }
        }
    }
}

static bool isWordOnBoard(Grid<char> & board, const string word, const uint8_t row, const uint8_t col) {

    const uint8_t len = word.length();

    if (len == 0) return true;

    for (int8_t i = -1; i <= 1; i++) {
        for (int8_t j = -1; j <= 1; j++) {

            if (board.inBounds(row + i, col + j)) {
                if (word[0] == board[row + i][col + j]) {

                    board[row + i][col + j] = '*';

                    if (isWordOnBoard(board, word.substr(1), row + i, col + j)) return true;
                    else board[row + i][col + j] = word[0];

                }
            }
        }
    }
    return false;
}

static bool isWordOnBoard(Grid<char> board, const string word) {

    const uint8_t size = board.numCols();
    const uint8_t len = word.length();

    if (len == 0) return false;

    for (uint8_t i = 0; i < size; i++) {
        for (uint8_t j = 0; j < size; j++) {

            if (word[0] == board[i][j]) {

                board[i][j] = '*';

                if (isWordOnBoard(board, word.substr(1), i, j)) return true;
                else board[i][j] = word[0];

            }
        }
    }
    return false;
}

int main() {
    GWindow gw(kBoggleWindowWidth, kBoggleWindowHeight);
    initGBoggle(gw);

    welcome();
    if (getYesOrNo("Do you need instructions?")) {
       giveInstructions();
    }

    do {

    const Grid<char> board = fillGrid();

    drawBoard(board.numRows(), board.numCols());

    showLetters(board);

    const Lexicon english("EnglishWords.dat");

    const Lexicon humanWords = humanTurn(english, board);

    computerTurn(english, board, humanWords);

    } while (getYesOrNo("Do you want to continue playing?"));

    return 0;
}
