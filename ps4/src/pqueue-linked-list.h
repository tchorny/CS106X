#ifndef _linked_list_pqueue_
#define _linked_list_pqueue_

#include "pqueue.h"
#include <string>
#include <cstddef>

class LinkedListPQueue : public PQueue {
public:
	LinkedListPQueue();
	~LinkedListPQueue();
	
	static LinkedListPQueue *merge(LinkedListPQueue *one, LinkedListPQueue *two);
	
	void enqueue(const std::string& elem);
    std::string extractMin();
    std::string peek() const;
	
private:
    struct List {
        std::string str;
        List * prev;
        List * next;
    };

    List * list;

    void freeList();

};

#endif
