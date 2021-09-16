#pragma once

#include <stdio.h>
#include <malloc.h>

#include "node.h"

typedef struct Queue
{
	Node* first;
	Node* current;
	int len;
} Queue;

// init empty queue. Need to destroy in the end of use
Queue* init_queue();

// destroy a queue. Get the queue and the destroy_func of the data that will be called when deleted a item
void destroy_queue(Queue* queue, destroy_data_ptr destroy_func);

// push new value to a queue. return the size of the queue and have error return -1
int push_queue(Queue* queue, void* data);

// Not free the return data. Return the last item. If have error return null
void* pop_queue(Queue* queue);

// return the size of the queue. Return -1 of queue is null
int len_queue(Queue* queue);

// return first value of the queue. Return null if have error 
void* first_value_queue(Queue* queue);
