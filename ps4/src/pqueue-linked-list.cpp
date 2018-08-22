#include "pqueue-linked-list.h"
using namespace std;

LinkedListPQueue::LinkedListPQueue() {
    list = new List;
    list->prev = NULL;
    list->next = NULL;
}
LinkedListPQueue::~LinkedListPQueue() { freeList(); }

string LinkedListPQueue::peek() const {
    if (logSize > 0) return list->str;
    return "";
}

string LinkedListPQueue::extractMin() {

    string peeked = peek();
    if (logSize > 1) {
        List * temp = list;
        list = list->next;
        list->prev = NULL;
        delete temp;
    }
    if (logSize > 0) logSize--;
    return peeked;
}

void LinkedListPQueue::enqueue(const string & elem) {

    if (logSize == 0) {
        list->str = elem;
        logSize++; return;
    }

    List * prev;

    for (List * tp = list; tp != NULL; tp = tp->next) {

        if (tp->str >= elem) {
            List * newlist = new List;
            newlist->str = elem;
            newlist->prev = tp->prev;
            newlist->next = tp;
            tp->prev = newlist;
            if (newlist->prev != NULL) newlist->prev->next = newlist;
            if (newlist->prev == NULL) list = newlist;
            logSize++;
            return;
        }
        prev = tp;
    }

    List * newlist = new List;
    newlist->str = elem;
    newlist->prev = prev;
    newlist->next = NULL;
    prev->next = newlist;
    logSize++;
}

LinkedListPQueue * LinkedListPQueue::merge(LinkedListPQueue * one, LinkedListPQueue * two) {

    LinkedListPQueue * newQueue = new LinkedListPQueue;

    List * tp1 = one->list;
    List * tp2 = two->list;
    List * tpMerged = newQueue->list;

    int sizeOneLeft = one->logSize;
    int sizeTwoLeft = two->logSize;

    while (((tp1 != NULL) || (tp2 != NULL)) && ((sizeOneLeft > 0) || (sizeTwoLeft > 0))) {

        List * newlist = new List;

        if (((tp1 == NULL) && (sizeTwoLeft > 0)) || ((tp1 != NULL) && (tp2 != NULL) && (sizeOneLeft > 0) && (sizeTwoLeft > 0) && (tp2->str <= tp1->str))) {

            if (newQueue->logSize == 0) {
                newQueue->list->str = tp2->str;
                tp2 = tp2->next;
                newQueue->logSize++;
                sizeTwoLeft--;
            }
            else {
            newlist->str = tp2->str;
            newlist->next = NULL;
            newlist->prev = tpMerged;
            tpMerged->next = newlist;

            tpMerged = tpMerged->next;
            tp2 = tp2->next;

            newQueue->logSize++;
            sizeTwoLeft--;
            }
        }
        else if (((tp2 == NULL) && (sizeOneLeft > 0)) || ((tp1 != NULL) && (tp2 != NULL) && (sizeOneLeft > 0) && (sizeTwoLeft > 0))) {

            if (newQueue->logSize == 0) {
                newQueue->list->str = tp1->str;
                tp1 = tp1->next;
                newQueue->logSize++;
                sizeOneLeft--;
            }
            else {
            newlist->str = tp1->str;
            newlist->next = NULL;
            newlist->prev = tpMerged;
            tpMerged->next = newlist;

            tpMerged = tpMerged->next;
            tp1 = tp1->next;

            newQueue->logSize++;
            sizeOneLeft--;
            }
        }
        else break;
    }
    return newQueue;
}

void LinkedListPQueue::freeList() {
    while (list != NULL) {
        List * next = list->next;
        delete list;
        list = next;
    }
}
