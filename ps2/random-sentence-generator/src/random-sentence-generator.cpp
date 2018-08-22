/**
 * File: random-sentence-generator.cpp
 * -----------------------------------
 * Presents a short program capable of reading in
 * context-free grammar files and generating arbitrary
 * sentences from them.
 */

#include <iostream>
#include <fstream>
#include <string>
using namespace std;

#include "console.h"
#include "simpio.h"   // for getLine
#include "strlib.h"   // for toLowerCase, trim
#include "map.h"

static const string kGrammarsDirectory = "grammars/";
static const string kGrammarFileExtension = ".g";
static string getNormalizedFilename(string filename) {
    string normalizedFileName = kGrammarsDirectory + filename;
    if (!endsWith(normalizedFileName, kGrammarFileExtension))
        normalizedFileName += kGrammarFileExtension;
    return normalizedFileName;
}

static bool isValidGrammarFilename(string filename) {
    string normalizedFileName = getNormalizedFilename(filename);
    ifstream infile(normalizedFileName.c_str());
    return !infile.fail();
}

static string getFileName() {
    while (true) {
        string filename = trim(getLine("Name of grammar file? [<return> to quit]: "));
        if (filename.empty() || isValidGrammarFilename(filename)) return filename;
        cout << "Failed to open the grammar file named \"" << filename << "\". Please try again...." << endl;
    }
}

//prototypes
bool isKeyLine(string line);
Vector<string> parseText(string & text);
Map<string, Vector<string>> buildMap (ifstream &infile);
void convertText (string & text, Map<string, Vector<string>> & map);


bool isKeyLine(string line) {

    line = trim(line);
    if ((stringLastIndexOf(line, "<") == 0) && stringIndexOf(line, ">") == (line.size() - 1)) return true;
    return false;
}

Vector<string> parseText(string & text) {

    Vector<string> vector;
    int start = -1, finish = -1;

    for (int i = 0; i < text.size(); i++) {

        if (text[i] == '<') start == i;
        else if (text[i] == '>') finish == i;

        if ((start != -1) && (finish != -1)) {

            vector.add(text.substr(start, finish - start + 1));
            start = finish = -1;
        }
    }

    return vector;
}

Map<string, Vector<string>> buildMap (ifstream & infile) {

    Map<string, Vector<string>> map;
    string line, newKey;
    int valuesLeft = 0;

    while (getline (infile, line)) {

        int dim;
        istringstream strstream (line);

        strstream >> dim >> ws;

        if (isKeyLine(line)) {

            newKey = line;
        }

        else if (!strstream.fail()) {

            Vector<string> tempVec;
            map.put(newKey, tempVec);
            valuesLeft = dim;
        }

        else if (valuesLeft > 0) {

            map[newKey].add(line);
            valuesLeft--;
        }
    }

    return map;
}

void convertText (string & text, Map<string, Vector<string>> & map) {

    while (stringLastIndexOf(text, ">") > 0 && text.substr(0, stringLastIndexOf(text, ">")).find("<") != string :: npos) {

        int pos = stringLastIndexOf(text.substr(0, stringLastIndexOf(text, ">")), "<");
        int n = stringLastIndexOf(text, ">") - stringLastIndexOf(text.substr(0, stringLastIndexOf(text, ">")), "<") + 1;

        string key = text.substr(pos, n);

        if (map.containsKey(key)) {

            shuffle(map[key]);
            text.replace(pos, n, map[key][0]);
        }
        else {
            cout << "Key " << key << " not found" << endl;
            break;
        }
    }
}

int main() {
    while (true) {

        string filename = getFileName();
        if (filename.empty()) break;
        cout << "Here's where you read in the \"" << filename << "\" grammar "
             << "and generate three random sentences." << endl << endl;

        ifstream infile;
        infile.open(getNormalizedFilename(filename).c_str());

        Map<string, Vector<string>> map;

        if (!infile.fail()) map = buildMap (infile);

        string text = map["<start>"][0];

        convertText(text, map);

        cout << text << endl << endl;
    }

    cout << "Thanks for playing!" << endl;
    return 0;


}
