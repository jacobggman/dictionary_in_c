#pragma once

#include <malloc.h>

typedef struct Array
{
	void* ptr;
	int len;
	int size;
} Array;


Array* array_init(int size, int len);
void array_destroy(Array* array);