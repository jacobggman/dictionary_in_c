#include "queue.h"

// init empty queue. Need to destroy in the end of use
Queue* init_queue()
{
	Queue* return_queue = malloc(sizeof(Queue));

	if (NULL == return_queue)
	{
		return NULL;
	}

	return_queue->len = 0;
	return_queue->current = NULL;
	return_queue->first = NULL;
	return return_queue;
}


// destroy a queue. Get the queue and the destroy_func of the data that will be called when deleted a item
void destroy_queue(Queue* queue, destroy_data_ptr destroy_func)
{
	if (NULL == queue)
	{
		return;
	}

	if (NULL == queue->first)
	{
		return;
	}

	destroy_nodes(queue->first, destroy_func);
	free(queue);
}

// push new value to a queue. return the size of the queue and have error return -1
int push_queue(Queue* queue, void* data)
{
	if (NULL == queue)
	{
		return -1;
	}
	
	Node* new_node = malloc(sizeof(Node));

	if (NULL == new_node)
	{
		return -1;
	}

	new_node->data = data;
	new_node->back = queue->current;
	new_node->next = NULL;

	
	queue->current->next = new_node;
	queue->current = new_node;
	return ++queue->len;
}

// Not free the return data. Return the last item. If have error return null
void* pop_queue(Queue* queue)
{
	if (NULL == queue)
	{
		return NULL;
	}

	if (NULL == queue->current)
	{
		return NULL;
	}

	Node* last_node = queue->current;
	queue->current = last_node->back;
	queue->current->next = NULL;
	queue->len--;

	void* data = last_node->data;
	free(last_node);
	return data;
}


// return the size of the queue. Return -1 of queue is null
int len_queue(Queue* queue)
{
	if (NULL == queue)
	{
		return -1;
	}
	return queue->len;
}

// return first value of the queue. Return null if have error 
void* first_value_queue(Queue* queue)
{
	if (NULL == queue)
	{
		return NULL;
	}

	Node* first = queue->first;
	if (NULL == first)
	{
		return NULL;
	}

	return first->data;
}




// TODO:
// add list
// add UI

