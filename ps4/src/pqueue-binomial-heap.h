#ifndef _binomial_heap_pqueue_
#define _binomial_heap_pqueue_

#include "pqueue.h"
#include <string>
#include "vector.h"
#include <iostream>

class BinomialHeapPQueue : public PQueue {
public:
	BinomialHeapPQueue();
	~BinomialHeapPQueue();
	
	static BinomialHeapPQueue *merge(BinomialHeapPQueue *one, BinomialHeapPQueue *two);
	
	void enqueue(const std::string& elem);
    std::string extractMin();
    std::string peek() const;
	
private:

    struct node {
        std::string str;
        Vector<node *> children;
    };

    Vector<node *> base;

    void destructNode(node * noode);

    static node * mergeTwoSameSizeNodes(node * one, node * two);

    static node * NodeDeepCopy(node * noode);
};

#endif
