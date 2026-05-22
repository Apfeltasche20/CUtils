#pragma once
#include <stdint.h>

typedef struct dynamic_array {
	uint8_t* buffer;
	uint64_t buffer_size;
} dynamic_array;

dynamic_array* dynamic_array_create();
void dynamic_array_set(dynamic_array* array, uint64_t index, uint8_t data);
uint8_t dynamic_array_get(dynamic_array* array, uint64_t index);
uint64_t dynamic_array_get_size(dynamic_array* array);
uint8_t* dynamic_array_get_static_copy(dynamic_array* array);
void dynamic_array_free(dynamic_array* array);

typedef struct dynamic_array_16 {
	uint16_t* buffer;
	uint64_t buffer_size;
} dynamic_array_16;

dynamic_array_16* dynamic_array_16_create();
void dynamic_array_16_set(dynamic_array_16* array, uint64_t index, uint16_t data);
uint16_t dynamic_array_16_get(dynamic_array_16* array, uint64_t index);
uint64_t dynamic_array_16_get_size(dynamic_array_16* array);
uint16_t* dynamic_array_16_get_static_copy(dynamic_array_16* array);
void dynamic_array_16_free(dynamic_array_16* array);