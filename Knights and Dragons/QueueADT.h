/*~*~*
   queue template
*~*/

#include <string>
#ifndef QUEUE_ADT
#define QUEUE_ADT


template <class Q>
class Queue
{
private:
	// Structure for the queue nodes
	struct QueueNode
	{
		Q value;          // Value in the node
		QueueNode* next = NULL;  // Pointer to next node
	};

	QueueNode* front = NULL;     // Pointer to the queue front
	QueueNode* rear = NULL;     // Pointer to the queue rear
	int length;

public:

	Queue() { length = 0; front = rear = front; }
	~Queue() {
		while (front != NULL) {
			dequeue();
		}
	}

	// Queue operations:

	/*
	This function will receive
	a value that that will be
	put into a new queue node
	and placed at the top of
	the queue
	*/
	void enqueue(Q val);


	/*
	This function will copy
	the value from the top
	queue node, delete that
	node (and change the queue
	to have a new top,) and then
	return the value copied
	*/
	Q dequeue();

	/*
	this function will return
	the value in the top queue
	node without affecting the
	queue in any other way
	*/
	Q peekFront() const {
		Q ret;
		if (front) {
			ret = front->value;
		}
		return ret;

	}


	Q peekRear() const {
		Q ret;
		if (rear) {
			ret = rear->value;
		}
		return ret;
	}


	/*
	This function will
	check to see if the
	top queue node is null
	if it is it will return
	true and if it is not
	then it will return false
	*/
	bool isEmpty() const {
		if (!front) {
			return true;
		}
		else {
			return false;
		}
	}

	int getLength() const { return length; }

};


/*~*~*
  Member function push inserts the argument onto
  the queue.
*~**/





/*~*~*
  Member function pop deletes the value at the top
  of the queue and returns it.
  Assume queue is not empty.
*~**/


#endif
