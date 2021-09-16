#include <string.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <ctype.h>

#include "array.h"
#include "dict.h"

#define NUM_ARGC 2
#define NO_ENOUGH_NUM_ARGC 1
#define CANT_READ_DICT 2
#define CANT_REPLACE_WORDS 3

#define LINE_BUFFER 256
#define WORD_BUFFER 256

Dict* read_dict(char* path);
int replace_words(Dict* dict, char* path);

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

void str_destory_void(void* str)
{
	free((char*)str);
}

void list_destroy_void(void* array)
{
	return list_destroy((List*)array, str_destory_void);
}

int main(int argc, char* argv[])
{
	srand(time(NULL));   // Initialization, should only be called once.

	int have_src_dest = argc > NUM_ARGC;
	if (0 == have_src_dest)
	{
		printf("Please enter source and destination file paths");
		return NO_ENOUGH_NUM_ARGC;
	}

	char* dest_path = argv[1];
	char* dict_path = argv[2];

	Dict* dict = read_dict(dict_path);

	if (NULL == dict)
	{
		printf("Can't read the dict file");
		return CANT_READ_DICT;
	}

	if (replace_words(dict, dest_path))
	{
		printf("Can't replace words");
		return CANT_REPLACE_WORDS;
	}
	// loop over all the words in the dest file

	// set all the words

	dict_free(dict);

	return 0;
}

int replace_words(Dict* dict, char* path)
{
	FILE* fp;
	if (NULL != fopen_s(&fp, path, "r"))
	{
		return NULL;
	}

	fseek(fp, 0L, SEEK_END);
	unsigned long long file_size = ftell(fp) - 1;
	rewind(fp);

	char* old_file_buff = malloc(sizeof(char) * file_size);
	if (NULL == old_file_buff)
	{
		return -1;
	}

	fread(old_file_buff, sizeof(char), file_size, fp);

	fclose(fp);

	// get word - 2
	// check if have - 1
	// random get it from the list - 3
	//
	// change the word and add padding/remove padding
	// if have too much -
	// if have too little -

	if (NULL != fopen_s(&fp, path, "w"))
	{
		free(old_file_buff);
		return -1;
	}

	unsigned long long space_index = 0;
	char word_buffer[WORD_BUFFER] = { 0 };
	int word_i = 0;
	while (space_index < file_size)
	{
		if (isspace(old_file_buff[space_index]) || space_index == file_size - 1) // or endoffile
		{
			char* word = word_buffer;
			if (strlen(word))
			{
				if (dict_find(dict, word))
				{
					List* list = dict_get(dict, word);
					int size = list->size;
					int random_index = rand() & size;
					if (random_index)
					{
						Node* first = list_first(list);
						random_index--;
						while (random_index)
						{
							first = list_next(first);
							random_index--;
						}
						word = first->data;
					}
				}
				fwrite(word, sizeof(char), strlen(word) * sizeof(char), fp);
			}
			char space[1] = { old_file_buff[space_index] };
			fwrite(space, sizeof(char), 1 * sizeof(char), fp);

			memset(word_buffer, 0, word_i);
			word_i = 0;
		}
		else
		{
			word_buffer[word_i] = old_file_buff[space_index];
			word_i++;
		}
		space_index++;
	}

	fclose(fp);

	free(old_file_buff);

	return 0;
}

// mine
//int hash_str(char* str, int size)
//{
//	int result = 0;
//	for (int size_of_key_word = 0; size_of_key_word < size; size_of_key_word++)
//	{
//		result += str[size_of_key_word] + str[size_of_key_word] * size_of_key_word;
//	}
//	return result;
//}

char* cut_str(char* buffer, int start_index, int endIndex)
{
	int len = endIndex - start_index;
	char* value = (char*)malloc(sizeof(char) * (len + 1)); // add null
	if (NULL == value)
	{
		return NULL;
	}
	memcpy(value, &(buffer[start_index]), len);
	value[len] = 0;

	return value;
}

List* read_values(char* buffer, int start_index)
{
	List* list = list_init();
	if (NULL == list)
	{
		return NULL;
	}

	int word_index = 0;
	int line_index = 0;
	while (1)
	{
		char c = buffer[word_index + start_index + line_index];
		if (0 == c || '\n' == c)
		{
			char* value = cut_str(buffer, start_index + line_index, start_index + line_index + word_index);
			if (NULL == value)
			{
				free(list);
				return NULL;
			}

			list_add(list, value);
			return list;
		}
		else if (',' == c)
		{
			char* value = cut_str(buffer, start_index + line_index, start_index + line_index + word_index);
			if (NULL == value)
			{
				free(list);
				return NULL;
			}

			list_add(list, value);
			line_index += word_index + 1;
			word_index = 0;
		}
		else
		{
			word_index++;
		}
	}
}

int size_of_key(char* buffer)
{
	int buffer_index = -1;
	while (1)
	{
		buffer_index++;
		if (buffer[buffer_index] == ':')
		{
			return buffer_index;
		}
	}
}

// return key, value
DictPair* read_line(FILE* file)
{
	char line[LINE_BUFFER] = { 0 };

	if (fgets(line, sizeof(line), file))
	{
		int buffer_index = size_of_key(line);

		if (0 == buffer_index)
		{
			return NULL;
		}

		char key_value[256];

		char* key = (char*)malloc(sizeof(char) * buffer_index + 1); // 1 for null
		if (NULL == key)
		{
			return NULL;
		}

		memcpy(key, line, buffer_index);
		key[buffer_index] = 0;

		List* values = read_values(line, buffer_index + 1);  // pass the :
		if (NULL == values)
		{
			free(key);
			return NULL;
		}

		return _init_pair(key, values);
	}
	else
	{
		return NULL;
	}
}

Dict* read_dict(char* path)
{
	DictFunctions dictFunc;
	dictFunc.hash_func = hash_str_void;
	dictFunc.comp_func = strcmp_void;
	dictFunc.destory_value = list_destroy_void;
	dictFunc.destory_key = str_destory_void;
	Dict* dict = init_dict(dictFunc);

	if (NULL == dict)
	{
		return NULL;
	}

	FILE* fp;
	if (NULL != fopen_s(&fp, path, "r"))
	{
		return NULL;
	}

	DictPair* new_pair;
	while (new_pair = read_line(fp))
	{
		dict_insert_pair(dict, new_pair);
		if (dict_find(dict, new_pair->key))
		{
			printf("FIND");
		}
	}

	fclose(fp);

	return dict;
}