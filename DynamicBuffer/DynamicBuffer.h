#pragma once
#include <stdint.h>

typedef struct dynamic_buffer {
	uint8_t* buffer;
	uint64_t buffer_size;
	uint64_t size;
} dynamic_buffer;

dynamic_buffer* dynamic_buffer_create();
void dynamic_buffer_add(dynamic_buffer* buffer, uint8_t data);
void dynamic_buffer_add_array(dynamic_buffer* buffer, uint8_t* data, uint64_t data_length);
void dynamic_buffer_set_index_8(dynamic_buffer* buffer, uint64_t index, uint8_t data);
void dynamic_buffer_set_index_16(dynamic_buffer* buffer, uint64_t index, uint16_t data);
void dynamic_buffer_set_index_32(dynamic_buffer* buffer, uint64_t index, uint32_t data);
uint8_t* dynamic_buffer_get_buffer(dynamic_buffer* buffer);
uint8_t* dynamic_buffer_copy_buffer(dynamic_buffer* buffer);
uint64_t dynamic_buffer_get_size(dynamic_buffer* buffer);
void dynamic_buffer_free(dynamic_buffer* buffer);