#include "node.h"

// destroy this node and all the nexts
void destroy_nodes(Node* node, destroy_data_ptr destroy_func)
{
	if (node->next)
	{
		destroy_nodes(node->next, destroy_func);
	}

	destroy_func(node->data);
	free(node);
}
