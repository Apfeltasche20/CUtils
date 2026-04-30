#pragma once
#include <stdint.h>

void* safe_malloc(uint64_t size);

typedef struct _array_list {
	uint32_t current_size;
	uint32_t current_max_size;
	uint64_t* current_array;
} array_list;

array_list* create_array_list();
void list_add(array_list* list, uint64_t value);
uint64_t list_get(array_list* list, uint32_t index);
uint32_t list_get_index(array_list* list, uint64_t value);
uint32_t list_has(array_list* list, uint64_t value);
uint32_t list_remove_value(array_list* list, uint64_t value);
uint64_t list_remove_index(array_list* list, uint32_t index);
uint32_t list_size(array_list* list);