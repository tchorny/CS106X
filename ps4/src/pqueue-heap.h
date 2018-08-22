#ifndef _binary_heap_pqueue_
#define _binary_heap_pqueue_

#include "pqueue.h"
#include <string>

using namespace std;

class HeapPQueue : public PQueue {
public:
	HeapPQueue();
	~HeapPQueue();
	
	static HeapPQueue *merge(HeapPQueue *one, HeapPQueue *two);
	
	void enqueue(const std::string& elem);
    std::string extractMin();
    std::string peek() const;
    
private:

    // Instance variables

    static const int INITIAL_CAPACITY = 10;
    string * array;
    int capacity;
    int bottomLeftNode (int node);
    int parentNode(int node);
    void Heapify(int node);
    void expandCapacity();

};

#endif
