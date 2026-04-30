#include "cutils.h"
#include "stdio.h"
#include "Windows.h"

#define LIST_START_SIZE 8
#define LIST_GROW_FACTOR 2
#define LIST_SCRINK_FACTOR 4

array_list* create_array_list_with_size(uint32_t size)
{
	array_list* array_list = safe_malloc(sizeof(struct _array_list));

	array_list->current_size = 0;
	array_list->current_max_size = size;
	array_list->current_array = safe_malloc(sizeof(uint64_t) * size);

	return (struct array_list*)array_list;
}

array_list* create_array_list()
{
	array_list* array_list = safe_malloc(sizeof(struct _array_list));

	array_list->current_size = 0;
	array_list->current_max_size = LIST_START_SIZE;
	array_list->current_array = safe_malloc(sizeof(uint64_t) * LIST_START_SIZE);

	return (struct array_list*) array_list;
}

void array_list_alloc_space(array_list* array_list, uint32_t size)
{
	if (size < array_list->current_size)
	{
		fprintf(stderr, "Array list can't be made size %i for %i entries\n", size, array_list->current_size);
		exit(-1);
	}
	
	void* new_array = safe_malloc(sizeof(uint64_t) * size);
	memcpy(new_array, array_list->current_array, array_list->current_size);
	free(array_list->current_array);

	array_list->current_array = new_array;
	array_list->current_max_size = size;
}

void array_list_scrink(array_list* array_list)
{
	array_list_alloc_space(array_list, array_list->current_max_size / LIST_GROW_FACTOR);
}

void array_list_grow(array_list* array_list)
{
	array_list_alloc_space(array_list, array_list->current_max_size * LIST_GROW_FACTOR);
}

void array_list_add(array_list* array_list, uint64_t value)
{
	if (array_list->current_size == array_list->current_max_size)
		array_list_grow(array_list);

	array_list->current_array[array_list->current_size] = value;
	array_list->current_size++;
}

uint64_t array_list_get(array_list* array_list, uint32_t index)
{
	if (index >= array_list->current_size)
	{
		fprintf(stderr, "Index out of Bounds %i for size %i\n", index, array_list->current_size);
		exit(-1);
	}

	return array_list->current_array[index];
}

uint32_t array_list_get_index(array_list* array_list, uint64_t value)
{
	for (int i = 0;i < array_list->current_size;i++)
	{
		if (array_list->current_array[i] == value)
			return i;
	}
	return -1;
}

uint32_t array_list_has(array_list* array_list, uint64_t value)
{
	for (int i = 0;i < array_list->current_size;i++)
	{
		if (array_list->current_array[i] == value)
			return 1;
	}
	return 0;
}

uint32_t array_list_remove_value(array_list* array_list, uint64_t value)
{
	uint32_t index = array_list_get_index(array_list, value);
	if (index == -1)
		return 0;
	array_list_remove_index(array_list, index);
	return index;
}

uint64_t array_list_remove_index(array_list* array_list, uint32_t index)
{
	if (index >= array_list->current_size)
	{
		fprintf(stderr, "Index out of Bounds %i for size %i\n", index, array_list->current_size);
		exit(-1);
	}
	
	uint64_t value = array_list->current_array[index];
	array_list->current_array[index] = 0;

	for (int i = (index + 1);i < array_list->current_size;i++)
	{
		array_list->current_array[index - i] = array_list->current_array[index];
	}
	array_list->current_size--;

	if((array_list->current_size * LIST_SCRINK_FACTOR) <= array_list->current_max_size)
		array_list_scrink(array_list);

	return value;
}

uint32_t array_list_size(array_list* array_list)
{
	return array_list->current_size;
}