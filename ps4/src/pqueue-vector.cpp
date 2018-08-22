#include "pqueue-vector.h"

using namespace std;

VectorPQueue::VectorPQueue() {}
VectorPQueue::~VectorPQueue() {}

string VectorPQueue::peek() const {

    if (vecQueue.isEmpty()) return "";

    string str = vecQueue[0];

    for (int i = 0; i < logSize; i++) {
        if (vecQueue[i] < str) str = vecQueue[i];
    }

    return str;
}

string VectorPQueue::extractMin() {

    if (vecQueue.isEmpty()) return "";

    string str = vecQueue[0];
    int index = 0;

    for (int i = 0; i < logSize; i++) {
        if (vecQueue[i] < str) {
            str = vecQueue[i];
            index = i;
        }
    }

    vecQueue.remove(index);
    logSize--;

    return str;
}

void VectorPQueue::enqueue(const string & elem) {
    vecQueue.add(elem);
    logSize++;
}

VectorPQueue *VectorPQueue::merge(VectorPQueue * one, VectorPQueue * two) {

    VectorPQueue * newQueue = new VectorPQueue;

    newQueue->logSize = (one->logSize) + (two->logSize);
    newQueue->vecQueue.addAll(one->vecQueue);
    newQueue->vecQueue.addAll(two->vecQueue);

    return newQueue;
}
