#include "cutils.h"
#include "stdio.h"
#include "Windows.h"

#define LIST_START_SIZE 8
#define LIST_GROW_FACTOR 2

array_list* create_array_list()
{
	array_list* list = safe_malloc(sizeof(array_list));

	list->current_size = 0;
	list->current_max_size = LIST_START_SIZE;
	list->current_array = safe_malloc(sizeof(uint64_t) * LIST_START_SIZE);

	return (struct list*) list;
}

void list_grow(array_list* list)
{
	uint32_t new_size = list->current_max_size * 2;

	void* new_array = safe_malloc(sizeof(uint64_t) * new_size);
	memcpy(new_array, list->current_array, list->current_size);
	free(list->current_array);

	list->current_array = new_array;
	list->current_max_size = new_size;
}

void list_add(array_list* list, uint64_t value)
{
	if (list->current_size == list->current_max_size)
		list_grow(list);

	list->current_array[list->current_size] = value;
	list->current_size++;
}

uint64_t list_get(array_list* list, uint32_t index)
{
	if (index >= list->current_size)
	{
		fprintf(stderr, "Index out of Bounds %i for size %i\n", index, list->current_size);
		exit(-1);
	}

	return list->current_array[index];
}

uint32_t list_get_index(array_list* list, uint64_t value)
{
	for (int i = 0;i < list->current_size;i++)
	{
		if (list->current_array[i] == value)
			return i;
	}
	return -1;
}

uint32_t list_has(array_list* list, uint64_t value)
{
	for (int i = 0;i < list->current_size;i++)
	{
		if (list->current_array[i] == value)
			return 1;
	}
	return 0;
}

uint32_t list_remove_value(array_list* list, uint64_t value)
{
	uint32_t index = list_get_index(list, value);
	if (index == -1)
		return 0;
	list_remove_index(list, index);
	return index;
}

uint64_t list_remove_index(array_list* list, uint32_t index)
{
	if (index >= list->current_size)
	{
		fprintf(stderr, "Index out of Bounds %i for size %i\n", index, list->current_size);
		exit(-1);
	}
	
	uint64_t value = list->current_array[index];
	list->current_array[index] = 0;

	for (int i = (index + 1);i < list->current_size;i++)
	{
		list->current_array[index - i] = list->current_array[index];
	}

	return value;
}

uint32_t list_size(array_list* list)
{
	return list->current_size;
}