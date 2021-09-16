#include <string.h>
#include <stdio.h>
#include "array.h"
#include "dict.h"

#define NUM_ARGC 2
#define NO_ENOUGH_NUM_ARGC 1

int main(int argc, char* argv[])
{
	int have_src_dest = argc < NUM_ARGC;
	if (0 == have_src_dest)
	{
		printf("Please enter source and destination file paths");
		return NO_ENOUGH_NUM_ARGC;
	}

	char* dest_path = argv[1];
	char* dict_path = argv[2];

	// read files data

	// set the data in dict

	// loop over all the words in the dest file

	// set all the words

	return 0;
}

// mine
//int hash_str(char* str, int size)
//{
//	int result = 0;
//	for (int i = 0; i < size; i++)
//	{
//		result += str[i] + str[i] * i;
//	}
//	return result;
//}

inline unsigned long hash_str(unsigned char* str)
{
	unsigned long hash = 5381;
	int c;

	while (c = *str++)
	{
		hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
	}



	return hash;
}


unsigned long hash_str_void(void* str)
{
	return hash_str((unsigned char*)str);
}

int strcmp_void(void* str1, void* str2)
{
	return strcmp((char*)str1, (char*)str2);
}

void array_destroy_void(void* array)
{
	return array_destroy((Array*)array);
}

void str_destory_void(void* array)
{
	// TODO
}

Dict* read_dile(char* path, int buffer_size)
{
	DictFunctions dictFunc;
	dictFunc.hash_func = hash_str_void;
	dictFunc.comp_func = strcmp_void;
	dictFunc.destory_value = array_destroy_void;
	dictFunc.destory_key = str_destory_void;
	Dict* dict = init_dict(dictFunc);

	FILE* fp;

	char* buff = (char*) malloc(sizeof(char) * buffer_size);

	List* list = init_list();
	fp = fopen(path, "r");

	int i = 0;
	while (true)
	{
		if (i >= buffer_size)
		{
			return NULL;
		}
		int c = getc(fp);

		if (EOF == c)
		{
			return NULL;
		}
		if (c == ':')
		{
			break;
		}
		buff[i] = c;
		i++;
	}

	add_to_list();
	
	fclose(fp);



	return dict;
}
char* read_line(FILE* file);


// todo:
// write vector
// write string hash map
// read file content
// format line of dict
// 
// 
// 
//example: word1, word2, word3
//example2: word1, word2, word3

// make list by asking and not before
