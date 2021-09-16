#include "dict.h"

void destory_nothing(void* _)
{
}

int dict_insert_pair(Dict* dict, DictPair* pair)
{
	if (dict->len > dict->element_per_bucket * dict->num_buckets)
	{
		if (0 == _dict_resize(dict, dict->element_per_bucket * 2))
		{
			return 0;
		}
	}

	unsigned long index = dict->functions.hash_func(pair->key);
	unsigned long capacity_index = index % dict->num_buckets;

	if (0 == list_add(dict->buckets[capacity_index], pair))
	{
		free(pair);
		return 0;
	}

	dict->len++;
	return 1;
}

void dict_reassign_pair(Dict* dict, void* key, void* new_value)
{
	unsigned long index = dict->functions.hash_func(key);
	unsigned long capacity_index = index % dict->num_buckets;
	List* list = dict->buckets[capacity_index];

	Node* now_node = list_first(list);
	while (NULL != now_node)
	{
		DictPair* pair = now_node->data;
		if (dict->functions.comp_func(pair->key, key) == 0)
		{
			dict->functions.destory_value(pair->value);
			pair->value = new_value;
			return;
		}
		now_node = list_next(now_node);
	}
}

void dict_remove(Dict* dict, void* key)
{
	unsigned long index = dict->functions.hash_func(key);
	unsigned long capacity_index = index % dict->num_buckets;
	List* list = dict->buckets[capacity_index];

	Node* now_node = list_first(list);
	while (NULL != now_node)
	{
		DictPair* pair = now_node->data;
		if (dict->functions.comp_func(pair->key, key) == 0)
		{
			dict->functions.destory_value(pair->value);
			dict->functions.destory_key(pair->key);
			free(pair);

			Node* back = now_node->back;
			Node* next = now_node->next;
			// switch
			back->next = next;
			if (NULL != next->back)
			{
				next->back = back;
			}
			free(now_node);
			list->size--;

			return;
		}
		now_node = list_next(now_node);
	}
}

int dict_find(Dict* dict, void* key)
{
	unsigned long index = dict->functions.hash_func(key);
	unsigned long capacity_index = index % dict->num_buckets;
	List* list = dict->buckets[capacity_index];

	Node* now_node = list_first(list);
	while (NULL != now_node)
	{
		DictPair* pair = now_node->data;
		int have_same_key = dict->functions.comp_func(pair->key, key) == 0;
		if (have_same_key)
		{
			return 1;
		}
		now_node = list_next(now_node);
	}
	return 0;
}

void* dict_get(Dict* dict, void* key)
{
	unsigned long index = dict->functions.hash_func(key);
	unsigned long capacity_index = index % dict->num_buckets;
	List* list = dict->buckets[capacity_index];

	Node* now_node = list_first(list);
	while (NULL != now_node)
	{
		DictPair* pair = now_node->data;
		if (dict->functions.comp_func(pair->key, key) == 0)
		{
			return pair->value;
		}
		now_node = list_next(now_node);
	}
	return NULL;
}

int dict_insert(Dict* dict, void* key, void* value)
{
	DictPair* pair = _init_pair(key, value);
	if (NULL == pair)
	{
		return 0;
	}
	return dict_insert_pair(dict, pair);
}

int _dict_resize(Dict* dict, int new_num_buckets)
{
	List** buckets = dict->buckets;
	int old_num_buckets = dict->num_buckets;
	dict->num_buckets = new_num_buckets;
	dict->len = 0;
	if (0 == _dict_init_all(dict))
	{
		return 0;
	}

	// add the old values
	for (int i = 0; i < old_num_buckets; i++)
	{
		List* list = buckets[i];
		Node* now_node = list_first(list);

		while (NULL != now_node)
		{
			dict_insert_pair(dict, now_node->data);
			now_node = list_next(now_node);
		}
		list_destroy(list, destory_nothing);
	}
	return 1;
}

int _dict_init_all(Dict* dict)
{
	dict->buckets = malloc(sizeof(List*) * dict->num_buckets);

	if (NULL == dict->buckets)
	{
		return 0;
	}

	for (int i = 0; i < dict->num_buckets; i++)
	{
		dict->buckets[i] = list_init();
		if (NULL == dict->buckets[i])
		{
			for (int j = 0; j < i; j++)
			{
				list_destroy(dict->buckets[j], destory_nothing);
			}
			free(dict->buckets);
			return 0;
		}
	}

	return 1;
}

void _dict_free_all(Dict* dict)
{
	PairAndFuncs pair;
	pair.destory_key = dict->functions.destory_key;
	pair.destory_value = dict->functions.destory_value;

	for (int i = 0; i < dict->len; i++)
	{
		List* bucket = dict->buckets[i];
		Node* node = list_first(bucket);
		while (NULL != node)
		{
			DictPair* pair = node->data;
			dict->functions.destory_key(pair->key);
			dict->functions.destory_value(pair->value);
			node = list_next(node);
		}
		list_destroy(dict->buckets[i], destory_nothing);
	}

	free(dict->buckets);
}

DictPair* _init_pair(void* key, void* value)
{
	DictPair* pair = malloc(sizeof(DictPair));
	if (NULL == pair)
	{
		return NULL;
	}

	pair->key = key;
	pair->value = value;

	return pair;
}

Dict* init_dict_pars(int start_num_buckets, int element_per_bucket, DictFunctions functions)
{
	Dict* dict = malloc(sizeof(Dict));

	if (NULL == dict)
	{
		return NULL;
	}

	dict->len = 0;
	dict->num_buckets = start_num_buckets;
	dict->element_per_bucket = element_per_bucket;
	dict->functions = functions;

	if (0 == _dict_init_all(dict))
	{
		free(dict);
		return NULL;
	}

	return dict;
}

Dict* init_dict(DictFunctions functions)
{
	return init_dict_pars(8, 50, functions);
}

void dict_free(Dict* dict)
{
	_dict_free_all(dict);
	free(dict);
}