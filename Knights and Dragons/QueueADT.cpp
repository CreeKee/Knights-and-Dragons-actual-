#include "QueueADT.h"


template <class Q>
void Queue<Q>::enqueue(Q val) {
	QueueNode* newNode; // Pointer to a new node

	// Allocate a new node and store num there.
	newNode = new QueueNode;
	if (!newNode)
		return;
	newNode->value = val;
	newNode->next = NULL;

	// Update links and counter
	if (front == NULL) // front is NULL: empty queue
		front = newNode;
	else
		rear->next = newNode;

	rear = newNode;
	length++;
}

template <class Q>
Q Queue<Q>::dequeue() {
	Q retVal;
	QueueNode* s = NULL;
	if (length != 0) {
		retVal = front->value;
		s = front->next;
		delete front;
		front = s;
		length--;
	}
	return retVal;
}