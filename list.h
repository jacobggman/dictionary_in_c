#pragma once

#include "node.h"
#include <malloc.h>

typedef struct List
{
	Node* start;
	Node* last;
	int size;
} List;

List* list_init();
void list_destroy(List* list, destroy_data_ptr func);

int list_add(List* list, void* value);
void list_remove(List* list, Node* node, destroy_data_ptr func);

Node* list_first(List* list);
Node* list_last(List* list);

Node* list_next(Node* node);
Node* list_back(Node* node);

int list_len(List* list);

//// return -1 if list null
//float average_list(List* list);

//// if ptr is not null
//void destory_int(void* ptr);