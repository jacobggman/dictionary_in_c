#include "array.h"


Array* array_init(int size, int len)
{
	Array* returnArray = malloc(sizeof(Array));
	if (NULL == returnArray)
	{
		return NULL;
	}
	returnArray->ptr = malloc(size * len);
	if (NULL == returnArray->ptr)
	{
		free(returnArray);
		return NULL;
	}
	returnArray->size = size;
	returnArray->len = len;
}

void array_destroy(Array* array)
{
	free(array->ptr);
	free(array);
}
