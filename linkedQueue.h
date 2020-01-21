//  Nicholas Poole
//  CS 302 - Section 1004 
//  Assignment #4

// The program reverse reverses the direction of an audio file so the user
// can listen to it either forwards or backwards. It does this using a queue 
// and a stack, both executed using linked lists, to reverse the order of 
// lines in a data file converted by the program sox from a .ogg audio file. 

// This file implements a queue with a linked list.

// *********************************************************************

#ifndef H_LINKEDQUEUE
#define H_LINKEDQUEUE
#define SIZE 32000
#include <iostream>
using namespace std;

// Node template
// Includes front, back counters for array
// front and back, dataSet array, and next-node link.
template <class myType>
struct queueNode{
	myType dataSet[SIZE];
	int front, back;
	queueNode<myType> *link;
};

template <class myType>
class linkedQueue
{
public:
	// constructor
	linkedQueue();
	// destructor
	~linkedQueue();
	// Checks whether queue is empty
	bool isEmptyQueue() const;
	// Initialize empty queue
	void initializeQueue();
	// Add element at back of queue
	void addItem(const myType& newItem);
	// Returns current element at front of queue
	myType front() const;
	// Returns current element at back of queue
	myType back() const;
	// Remove element from front of queue
	void deleteItem();
	// Returns current number of elements in queue
	int queueCount();
	// Prints all elements to terminal
	void printQueue();
private:
	// Front of queue node
	queueNode<myType> *queueFront;
	// Back of queue node
	queueNode<myType> *queueRear;
	// Number of elements in queue
	int count;
};

/*---------------------------------------
Constructor
PARAMETER(S):	N/A
POSTCONDITION(S): Sets queueFront, QueueRear
	to NULL, count to zero.
---------------------------------------*/
template <class myType>
linkedQueue<myType>::linkedQueue()
{
	queueFront = NULL;
	queueRear = NULL;
	count = 0;
}

/*---------------------------------------
Destructor
PARAMETER(S):	N/A
POSTCONDITION(S): Deletes all elements of queue,
	sets queueFront and queueRear to NULL.
---------------------------------------*/
template <class myType>
linkedQueue<myType>::~linkedQueue()
{
	while (!isEmptyQueue())
		deleteItem();
	queueFront = NULL;
	queueRear = NULL;
}

/*---------------------------------------
FUNCTION NAME:	isEmptyQueue
PARAMETER(S):	N/A
RETURN TYPE:	bool
POSTCONDITION(S): Returns TRUE if queueFront is NULL,
	else returns FALSE.
---------------------------------------*/
template <class myType>
bool linkedQueue<myType>::isEmptyQueue() const
{
	return queueFront == NULL;
}

/*---------------------------------------
FUNCTION NAME:	initializeQueue
PARAMETER(S):	N/A
RETURN TYPE:	void
POSTCONDITION(S): Initializes front and rear nodes
	of empty queue. If not empty, empties queue.
	Sets count to zero.
---------------------------------------*/
template <class myType>
void linkedQueue<myType>::initializeQueue()
{
	if (!isEmptyQueue())
		deleteItem();
	queueFront = NULL;
	queueRear = NULL;
	count=0;
}

/*---------------------------------------
FUNCTION NAME:	addItem
PARAMETER(S):	const myType& newItem (item to be passed in)
RETURN TYPE:	void
POSTCONDITION(S): Adds member to node array and increments count.
	Adds new node if current array is full or if queue is empty.
---------------------------------------*/
template <class myType>
void linkedQueue<myType>::addItem(const myType& newItem)
{
	//Add first item to empty queue
	if(isEmptyQueue())
	{
		queueNode<myType> *temp;
		temp = new queueNode<myType>;
		temp -> front = 0;
		temp -> back = 0;
		temp -> dataSet[temp -> back] = newItem;
		temp -> link = queueFront;
		queueFront = temp;
		queueRear = temp;
		count++;
	}
	else
	{
		//Add item to array within node
		if (queueRear -> back < SIZE-1)
		{
			queueRear -> back += 1;
			queueRear -> dataSet[queueRear -> back] = newItem;
			count++;
		}
		//Node is full, create new node and add first item
		else if (queueRear -> back == SIZE-1)
		{
			queueNode<myType> *temp;
			temp = new queueNode<myType>;
			temp -> front = 0;
			temp -> back = 0;
			temp -> dataSet[temp -> back] = newItem;
			queueRear -> link = temp;
			queueRear = temp;
			queueRear -> link = queueFront;
			count++;
		}
	}
}

/*---------------------------------------
FUNCTION NAME:	front
PARAMETER(S):	N/A
RETURN TYPE:	myType
POSTCONDITION(S): Returns current front of the queue.
---------------------------------------*/
template <class myType>
myType linkedQueue<myType>::front() const
{
	return queueFront -> dataSet[queueFront -> front];
}

/*---------------------------------------
FUNCTION NAME:	back
PARAMETER(S):	N/A
RETURN TYPE:	myType
POSTCONDITION(S): Returns current back of the queue.
---------------------------------------*/
template <class myType>
myType linkedQueue<myType>::back() const
{
	return queueRear -> dataSet[queueRear -> back];
}

/*---------------------------------------
FUNCTION NAME:	deleteItem
PARAMETER(S):	N/A
RETURN TYPE:	void
POSTCONDITION(S): Increments front of current node array,
	effectively removing the member from the queue. Deletes
	nodes as arrays are emptied.
---------------------------------------*/
template <class myType>
void linkedQueue<myType>::deleteItem()
{

	if(!isEmptyQueue())
	{
		//De-populating array.
		if (queueFront -> front < queueFront -> back)
		{
			queueFront -> front += 1;
			count--;
		}
		//Queue emptied, delete node, move to next node.
		else if(queueFront -> front == queueFront -> back && queueFront != queueRear)//queueFront -> link != NULL)
		{
			queueNode<myType> *temp;
			temp = queueFront -> link;
			delete queueFront;
			queueFront = temp;
			count--;
		}
		//Final node, queue emptied, delete node.
		else if(queueFront -> front == queueFront -> back && queueFront == queueRear)//queueFront -> link == NULL)
		{
			delete queueFront;
			queueFront = NULL;
			queueRear = NULL;
			count--;
		}
	}
}

/*---------------------------------------
FUNCTION NAME:	queueCount
PARAMETER(S):	N/A
RETURN TYPE:	int
POSTCONDITION(S): Returns current number of queue members
---------------------------------------*/
template <class myType>
int linkedQueue<myType>::queueCount()
{
	return count;
}

/*---------------------------------------
FUNCTION NAME:	printQueue
PARAMETER(S):	N/A
RETURN TYPE:	void
POSTCONDITION(S): Prints all queue members to terminal.
---------------------------------------*/
template <class myType>
void linkedQueue<myType>::printQueue()
{
	int count = 0;
	queueNode<myType> *counter;

	if (!isEmptyQueue())
	{
		int countBack = queueRear -> back;
		counter = queueFront;

		while (count <= countBack)
		{
			cout << counter -> dataSet[count] << " ";
			count++;
			
			if (counter -> link != queueFront && queueFront != queueRear)
			{
				counter = counter -> link;
				countBack = counter -> back;
			}
		}	
	}
}

#endif