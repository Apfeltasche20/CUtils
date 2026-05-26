#pragma once
#include <stdint.h>

typedef struct _array_list {
	uint32_t current_size;
	uint32_t current_max_size;
	uint64_t* current_array;
} array_list;

array_list* array_list_create();
array_list* array_list_create_with_size(uint32_t size);
void array_list_add(array_list* array_list, uint64_t value);
void array_list_alloc_space(array_list* array_list, uint32_t size);
uint64_t array_list_get(array_list* array_list, uint32_t index);
uint32_t array_list_get_index(array_list* array_list, uint64_t value);
uint32_t array_list_has(array_list* array_list, uint64_t value);
uint32_t array_list_remove_value(array_list* array_list, uint64_t value);
uint64_t array_list_remove_index(array_list* array_list, uint32_t index);
uint32_t array_list_size(array_list* array_list);