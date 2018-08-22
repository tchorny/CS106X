#include "pqueue-heap.h"
using namespace std;

HeapPQueue::HeapPQueue() {
    capacity = INITIAL_CAPACITY;
    array = new string[capacity];
}
HeapPQueue::~HeapPQueue() {
    delete[] array;
}

string HeapPQueue::peek() const {
    if (logSize > 0) return array[0];
	return "";
}

string HeapPQueue::extractMin() {

    string peeked = peek();

    if (logSize > 0) {
        array[0] = array[(logSize--) - 1];
        this->Heapify(0);
    }
    return peeked;
}

void HeapPQueue::enqueue(const string & elem) {

    if (capacity == logSize) expandCapacity();

    array[logSize++] = elem;

    //i everywhere here - index inside 1-based array, j - index inside 0-based array
    for (int j = (logSize - 1), i = parentNode(j) + 1; parentNode(j) < j; i /= 2) {
        if (array[j] < array[i - 1]) {
            string temp = array[i - 1];
            array[i - 1] = array[j];
            array[j] = temp;
            j = i - 1;
        }
        else break;
    }
}

HeapPQueue *HeapPQueue::merge(HeapPQueue * one, HeapPQueue * two) {

    HeapPQueue * newQueue = new HeapPQueue;
    newQueue->logSize = one->logSize + two->logSize;
    newQueue->capacity = newQueue->logSize;
    delete[] newQueue->array;
    newQueue->array = new string[newQueue->capacity];

    for (int i = 0; i < one->logSize; i++) {
        newQueue->array[i] = one->array[i];
    }
    for (int i = 0; i < two->logSize; i++) {
        newQueue->array[one->logSize + i] = two->array[i];
    }
    for (int i = newQueue->logSize - 1; i >= 0; i--){
        newQueue->Heapify(i);
    }
    return newQueue;
}

int HeapPQueue::bottomLeftNode(int node) {
    return 2 * (node + 1) - 1;
}

int HeapPQueue::parentNode(int node) {
    if (node == 0) return 0;
    return (node + 1) / 2 - 1;
}

void HeapPQueue::Heapify(int node) {

    if (bottomLeftNode(node) < logSize) {

        //i everywhere here - index inside 1-based array, j - index inside 0-based array
        for (int j = node, i = bottomLeftNode(node) + 1; (i - 1) < logSize; i *= 2) {

            //if right bottom node exist
            if ((i < logSize) && (array[i] < array[i - 1]) && (array[i] < array[j])) {
                i++;
                string temp = array[j];
                array[j] = array[i - 1];
                array[i - 1] = temp;
                j = i - 1;
            }
            else if (array[i - 1] < array[j]) {
                string temp = array[j];
                array[j] = array[i - 1];
                array[i - 1] = temp;
                j = i - 1;
            }
            else break;
        }
    }
}

void HeapPQueue::expandCapacity() {

    string * oldArray = array;
    capacity *= 2;
    array = new string[capacity];
    for (int i = 0; i < logSize; i++) {
        array[i] = oldArray[i];
    }

    delete[] oldArray;
}
