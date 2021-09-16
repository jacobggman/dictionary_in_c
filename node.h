#pragma once

typedef struct Node Node;

typedef struct Node
{
	void* data;
	Node* next;
	Node* back;
} Node;

// function that destroy the void* 
typedef void (*destroy_data_ptr)(void*);

// destroy this node and all the nexts
void destroy_nodes(Node* node, destroy_data_ptr destroy_func);