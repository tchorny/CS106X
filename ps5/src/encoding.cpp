#include "encoding.h"
#include "pqueue.h"
#include "bitstream.h"
#include "filelib.h"
#include <bitset>

using namespace std;

Map<int, int> buildFrequencyTable(istream& input) {

    char c;
    Map<int, int> freqTable;

    while (input.get(c)) freqTable[(int)c]++;
    freqTable[PSEUDO_EOF]++;

    return freqTable;
}

HuffmanNode* buildEncodingTree(const Map<int, int>& freqTable) {

    PriorityQueue<HuffmanNode*> pq;
    for (int key : freqTable) {

        HuffmanNode* node = new HuffmanNode(key, freqTable[key], NULL, NULL);
        pq.enqueue(node, node->count);
    }
    while (pq.size() > 1) {

        HuffmanNode* left = pq.dequeue();
        HuffmanNode* right = pq.dequeue();
        HuffmanNode* branch = new HuffmanNode(NOT_A_CHAR, (left->count + right->count), left, right);
        pq.enqueue(branch, branch->count);
    }
    if ((pq.size() == 1) && (pq.peek() != NULL) && (pq.peek()->character != NOT_A_CHAR)) {
        HuffmanNode* node = pq.dequeue();
        HuffmanNode* newnode = new HuffmanNode(NOT_A_CHAR, node->count, node, NULL);
        pq.enqueue(newnode, newnode->count);
    }
    return (pq.isEmpty()) ? NULL : pq.dequeue();
}

Map<int, string> buildEncodingMap(HuffmanNode* encodingTree) {

    Map<int, string> encodingMap;
    if (encodingTree != NULL) treeTraversal(encodingTree, encodingMap, "");

    return encodingMap;
}

void encodeData(istream& input, const Map<int, string>& encodingMap, obitstream& output) {

    char c;
    while (input.get(c)) {

        int key = (int)c;
        if (encodingMap.containsKey(c)) {

            string str = encodingMap[key];
            for (int i = 0; i < str.length(); i++) {
                if (str[i] == '0') output.writeBit(0);
                else if (str[i] == '1') output.writeBit(1);
            }
        }
    }

    if (encodingMap.containsKey(PSEUDO_EOF)) {

        string str = encodingMap[PSEUDO_EOF];
        for (int i = 0; i < str.length(); i++) {
            if (str[i] == '0') output.writeBit(0);
            else if (str[i] == '1') output.writeBit(1);
        }
    }
}

void decodeData(ibitstream& input, HuffmanNode* encodingTree, ostream& output) {

    HuffmanNode* curr = encodingTree;
    while (true) {

        int i = input.readBit();
        if ((i == 0) && (curr->zero != NULL)) {
            curr = curr->zero;
            if (curr->character != NOT_A_CHAR) {
                if (curr->character == PSEUDO_EOF) return;
                output.put((char)curr->character);
                curr = encodingTree;
            }
        }
        else if ((i == 1) && (curr->one != NULL)) {
            curr = curr->one;
            if (curr->character != NOT_A_CHAR) {
                if (curr->character == PSEUDO_EOF) return;
                output.put((char)curr->character);
                curr = encodingTree;
            }
        } else return;
    }
}

void compress(istream& input, obitstream& output) {

    Map<int, int> freqTable = buildFrequencyTable(input);
    HuffmanNode* encodingTree = buildEncodingTree(freqTable);
    Map<int, string> encodingMap = buildEncodingMap(encodingTree);

    output << freqTable.toString();

    rewindStream(input);
    encodeData(input, encodingMap, output);
}

void decompress(ibitstream& input, ostream& output) {

    if (!isValidFile(input)) {
        cout << "Wrong encoded file format" << endl;
        return;
    }

    Map<int, int> freqTable;

    if (buildFreqTableFromHeader(input, freqTable) != 0) {
        cout << "Wrong encoded file format" << endl;
        return;
    }
    HuffmanNode* encodingTree = buildEncodingTree(freqTable);

    decodeData(input, encodingTree, output);
}

void freeTree(HuffmanNode* node) {
    if (node == NULL) return;
    freeTree(node->zero);
    freeTree(node->one);
    delete node;
}

void treeTraversal(HuffmanNode*& encodingTree, Map<int, string>& encodingMap, string str) {

    if (encodingTree->character == NOT_A_CHAR) {
        if (encodingTree->zero != NULL) treeTraversal(encodingTree->zero, encodingMap, str + "0");
        if (encodingTree->one != NULL) treeTraversal(encodingTree->one, encodingMap, str + "1");
    }
    else encodingMap.put(encodingTree->character, str);
}

bool isValidFile(ibitstream& input) {

    bitset<8> bitchar;
    for (int i = 0; i < 8; i++) {
        int bit = input.readBit();
        if ((bit == 0) || (bit == 1)) bitchar[i] = bit;
        else return false;
    }

    char ch = (char)bitchar.to_ulong();
    if (ch == '{') return true;
    else return false;
}

int buildFreqTableFromHeader(ibitstream& input, Map<int, int>& freqTable) {

    bitset<8> bitchar;
    int nextpos = 0;
    char ch = '{';

    int key, value;
    key = value = 0;
    bool isValue = false;
    bool isNegativeKey = false;

    while (ch != '}') {
        int bit = input.readBit();

        if ((bit == 0) || (bit == 1)) {

            bitchar[nextpos] = bit;
            nextpos = (nextpos + 1) % 8;

            if (nextpos == 0) {

                int intchar = bitchar.to_ulong();
                ch = (char)intchar;

                //if char code a number
                if (((intchar - 48) >= 0) && ((intchar - 48) <= 9)) {
                    if (isValue) value = value * 10 + (intchar - 48);
                    else key = key * 10 + (intchar - 48);
                } else {
                    switch (ch) {
                    case ',':
                    case '}':
                        (isNegativeKey) ? freqTable.put(-key, value) : freqTable.put(key, value);
                        isNegativeKey = isValue = false;
                        key = value = 0;
                        break;
                    case ':':
                        isValue = true;
                        break;
                    case '-':
                        isNegativeKey = true;
                        break;
                    case ' ': break;
                    default:
                        return 1; break;
                    }
                }
            }
        } else return 1;
    }
    return 0;
}
