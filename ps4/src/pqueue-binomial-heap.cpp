#include "pqueue-binomial-heap.h"

using namespace std;

BinomialHeapPQueue::BinomialHeapPQueue() {}
BinomialHeapPQueue::~BinomialHeapPQueue() {
    for (int i = 0; i < base.size(); i++) {
        destructNode(base[i]);
    }
}

string BinomialHeapPQueue::peek() const {
    if (logSize > 0) {

        string str;
        bool foundFirstStr = false;

        for (int i = 0; i < base.size(); i++) {
            if (base[i] != NULL) {
                if (foundFirstStr && ((base[i]->str) < str)) str = base[i]->str;
                else if (!foundFirstStr) {
                    str = base[i]->str; foundFirstStr = true;
                }
            }
        }
        return str;
    }
    return "";
}

string BinomialHeapPQueue::extractMin() {

    if (logSize > 0) {

        string str;
        bool foundFirstStr = false;
        int minIndex = 0;

        for (int i = 0; i < base.size(); i++) {
            if (base[i] != NULL) {
                if (foundFirstStr && ((base[i]->str) < str)) {
                    str = base[i]->str;
                    minIndex = i;
                }
                else if (!foundFirstStr) {
                    str = base[i]->str;
                    foundFirstStr = true;
                    minIndex = i;
                }
            }
        }

        node * tempnode = base[minIndex];
        base[minIndex] = NULL;

        for (int j = 0; j < tempnode->children.size(); j++) {

            int k = j;
            node * tempInnerNode = tempnode->children[j];

            while ((k < base.size()) && (tempInnerNode != NULL)) {

                if (base[k] == NULL) {
                    base[k] = tempInnerNode;
                    tempInnerNode = NULL;
                }
                else {
                    tempInnerNode = mergeTwoSameSizeNodes(base[k], tempInnerNode);
                    base[k] = NULL;
                }
                k++;
            }
        }
        delete tempnode;
        logSize--;

        return str;
    }
    return "";
}

void BinomialHeapPQueue::enqueue(const string & elem) {

    node * newnode = new node;
    newnode->str = elem;

    if (logSize == 0) {

        base.add(newnode);
        logSize++; return;
    }

    else if (!base.isEmpty()) {

        int i = 0;
        while ((i < base.size()) && (base[i] != NULL)) {
            newnode = mergeTwoSameSizeNodes(newnode, base[i]);
            base[i] = NULL;
            i++;
        }

        if (i < base.size()) base[i] = newnode;
        else base.add(newnode);
        logSize++;
    }
}

BinomialHeapPQueue * BinomialHeapPQueue::merge(BinomialHeapPQueue * one, BinomialHeapPQueue * two) {

    BinomialHeapPQueue * newQueue = new BinomialHeapPQueue;
    newQueue->logSize = one->logSize + two->logSize;

    node * tempnode = NULL;
    int i = 0;

    while ((i < one->base.size()) || (i < two->base.size())) {

        if ((i < one->base.size()) && (one->base[i] != NULL) && (i < two->base.size()) && (two->base[i] != NULL)) {

            newQueue->base.add(tempnode);
            tempnode = mergeTwoSameSizeNodes(NodeDeepCopy(one->base[i]), NodeDeepCopy(two->base[i]));
        }
        else if ((i < one->base.size()) && (one->base[i] != NULL)) {

            if (tempnode != NULL) {
                newQueue->base.add(NULL);
                tempnode = mergeTwoSameSizeNodes(NodeDeepCopy(one->base[i]), NodeDeepCopy(tempnode));
            }
            else newQueue->base.add(one->base[i]);
        }
        else if ((i < two->base.size()) && (two->base[i] != NULL)) {

            if (tempnode != NULL) {
                newQueue->base.add(NULL);
                tempnode = mergeTwoSameSizeNodes(NodeDeepCopy(two->base[i]), NodeDeepCopy(tempnode));
            }
            else newQueue->base.add(two->base[i]);
        }
        else {
            if (tempnode != NULL) {
                newQueue->base.add(tempnode);
                tempnode = NULL;
            }
            else newQueue->base.add(NULL);
        }
        i++;
    }

    if (tempnode != NULL) newQueue->base.add(tempnode);

    return newQueue;
}

void BinomialHeapPQueue::destructNode(node * noode) {
    if (noode != NULL) {
        for (int i = 0; i < noode->children.size(); i++) {
            destructNode(noode->children[i]);
            delete noode;
        }
    }
}

BinomialHeapPQueue::node * BinomialHeapPQueue::mergeTwoSameSizeNodes(node * one, node * two) {

    if (one->str >= two->str) {
        two->children.add(one);
        return two;
    }
    else {
        one->children.add(two);
        return one;
    }
}

BinomialHeapPQueue::node * BinomialHeapPQueue::NodeDeepCopy(node * noode) {

    if (noode == NULL) return NULL;

    node * newnode = new node;
    newnode->str = noode->str;

    for (int i = 0; i < noode->children.size(); i++) {

        if (noode->children[i] != NULL) newnode->children.add(NodeDeepCopy(noode->children[i]));
    }
    return newnode;
}
