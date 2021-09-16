#pragma once

#include "list.h"

typedef unsigned long (*hash_data)(void*);
typedef int (*comp_data)(void*, void*);

typedef struct DictFunctions
{
	hash_data hash_func;
	comp_data comp_func;
	destroy_data_ptr destory_key;
	destroy_data_ptr destory_value;
} DictFunctions;

typedef struct DictPair
{
	void* key;
	void* value;
} DictPair;

typedef struct PairAndFuncs
{
	DictPair* DictPair;
	destroy_data_ptr destory_key;
	destroy_data_ptr destory_value;
} PairAndFuncs;

typedef struct Dict
{
	int len;

	int num_buckets;
	List** buckets;
	int element_per_bucket; // between: 5 and 100
	DictFunctions functions;
} Dict;

typedef struct Line
{
	char* key;
	List list;
} StrDict;

Dict* init_dict_pars(int start_capacity, int element_per_bucket, DictFunctions functions);
Dict* init_dict(DictFunctions functions);
void dict_free(Dict* dict);
// void dict_set_pair(Dict* dict, DictPair* pair);
int dict_insert(Dict* dict, void* key, void* value);
int dict_insert_pair(Dict* dict, DictPair* pair);
void dict_reassign_pair(Dict* dict, void* key, void* newValue);
void dict_remove(Dict* dict, void* key);
int dict_find(Dict* dict, void* key);
void* dict_get(Dict* dict, void* key);
int _dict_resize(Dict* dict, int new_num_buckets);
int _dict_init_all(Dict* dict);
void _dict_free_all(Dict* dict);
DictPair* _init_pair(void* key, void* value);
void destory_nothing(void*);
