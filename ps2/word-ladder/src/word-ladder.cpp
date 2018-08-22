/**
 * File: word-ladder.cpp
 * ---------------------
 * Implements a program to find word ladders connecting pairs of words.
 */

#include <iostream>
using namespace std;

#include "console.h"
#include "lexicon.h"
#include "strlib.h"
#include "simpio.h"
#include "vector.h"
#include "queue.h"



void wordTransformation(string word, Lexicon & english, Lexicon & usedWords, vector<string> & newWords) {

    for (int i = 0; i < word.length(); i++) {

        for (char j = 'a'; j <= 'z'; j++) {

            if (j != word[i]) {

                string temp = word;
                temp[i] = j;

                if (english.contains(temp) && !usedWords.contains(temp)) {

                    newWords.push_back(temp);
                }
            }
        }
    }
}


static string getWord(Lexicon & english, string prompt) {
    while (true) {
        string response = trim(toLowerCase(getLine(prompt)));
        if (response.empty() || english.contains(response)) return response;
        cout << "Your response needs to be an English word, so please try again." << endl;
    }
}

static void generateLadder(Lexicon & english, string start, string end) {
    cout << "Here's where you'll search for a word ladder connecting \"" << start << "\" to \"" << end << "." << endl;


    Lexicon usedWords;

    Queue<vector<string>> queue;

    vector<string> startLadder(1, start);
    queue.enqueue(startLadder);

    bool ladderFound = false;

    while (!queue.isEmpty()) {

        vector<string> dequeued = queue.dequeue();

        if (dequeued[dequeued.size() - 1] == end) {

            cout << endl;
            for(int i = 0; i < dequeued.size(); i++) {
                cout << dequeued[i] << " ";
            }
            cout << endl << endl;
            ladderFound = true;
            break;
        }
        else {

            vector<string> newWords;
            wordTransformation(dequeued[dequeued.size() - 1], english, usedWords, newWords);

            for(int i = 0; i < newWords.size(); i++) {

                vector<string> temp = dequeued;
                temp.push_back(newWords[i]);
                queue.enqueue(temp);
                usedWords.add(newWords[i]);
            }
        }
    }

    if (!ladderFound) cout << endl << "No such ladder exist" << endl << endl;

}

static const string kEnglishLanguageDatafile = "EnglishWords.dat";

static void playWordLadder() {
    Lexicon english(kEnglishLanguageDatafile); //Creates English lexicon

    while (true) {
        string start = getWord(english, "Please enter the source word [return to quit]: ");
        if (start.empty()) break;
        string end = getWord(english, "Please enter the destination word [return to quit]: ");
        if (end.empty()) break;
        generateLadder(english, start, end);
    }



}


int main() {
    cout << "Welcome to the CS106 word ladder application!" << endl << endl;
    playWordLadder();
    cout << "Thanks for playing!" << endl;
    return 0;
}

