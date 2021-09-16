#include "list.h"

List* list_init()
{
	List* return_list = malloc(sizeof(List));

	if (NULL == return_list)
	{
		return NULL;
	}

	Node* start_node = malloc(sizeof(Node));
	if (NULL == start_node)
	{
		free(return_list);
		return NULL;
	}

	start_node->back = NULL;
	start_node->next = NULL;
	start_node->data = NULL;

	return_list->size = 0;
	return_list->start = start_node;
	return_list->last = start_node;
	return return_list;
}

void list_destroy(List* list, destroy_data_ptr func)
{
	if (NULL == list)
	{
		return;
	}

	destroy_nodes(list->start, func);
	free(list);
}

//void destory_int(void* ptr)
//{
//	if (NULL == ptr)
//	{
//		return;
//	}
//
//	int* int_ptr = (int*)ptr;
//	free(int_ptr);
//}

int list_add(List* list, void* value)
{
	if (NULL == list)
	{
		return 0;
	}

	Node* new_node = malloc(sizeof(Node));
	if (NULL == new_node)
	{
		return 0;
	}

	/*int* new_int = malloc(sizeof(int));
	if (NULL == new_int)
	{
		free(new_node);
		return;
	}
	*new_int = value;*/

	new_node->data = value;
	new_node->next = NULL;
	new_node->back = list->last;

	list->size++;
	list->last->next = new_node;
	list->last = new_node;

	return 1;
}

void list_remove(List* list, Node* node, destroy_data_ptr func)
{
	if (NULL == node)
	{
		return;
	}

	if (NULL == node->data)
	{
		return;
	}

	if (NULL == node->back)
	{
		return;
	}

	Node* back = node->back;
	Node* next = node->next;

	// switch
	back->next = next;
	if (NULL != next->back)
	{
		next->back = back;
	}

	// int* int_ptr = (int*)node->data;
	// int value = *int_ptr;
	func(node->data);
	free(node);

	list->size--;
}

Node* list_first(List* list)
{
	if (NULL == list)
	{
		return NULL;
	}

	// start is saved
	return list->start->next;
}

Node* list_last(List* list)
{
	if (NULL == list)
	{
		return NULL;
	}

	// start is saved
	return list->last->back;
}

Node* list_next(Node* node)
{
	if (NULL == node)
	{
		return NULL;
	}
	return node->next;
}

Node* list_back(Node* node)
{
	if (NULL == node)
	{
		return NULL;
	}
	return node->back;
}

int list_len(List* list)
{
	if (NULL == list)
	{
		return NULL;
	}
	return list->size;
}

//float average_list(List* list)
//{
//	if (NULL == list)
//	{
//		return -1;
//	}
//
//	int size = list->size;
//	if (size == 0)
//	{
//		return 0;
//	}
//	return list->total / size;
//}