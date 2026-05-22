#include "DynamicArray.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DYNAMIC_ARRAY_START_SIZE 8
#define DYNAMIC_ARRAY_GROW_RATE 2

dynamic_array* dynamic_array_create()
{
	dynamic_array* buffer = malloc(sizeof(struct dynamic_array));
	if (buffer == 0)
	{
		printf("Error Allocating Buffer\n");
		exit(-1);
	}

	buffer->buffer = malloc(DYNAMIC_ARRAY_START_SIZE);
	if (buffer->buffer == 0)
	{
		free(buffer);
		printf("Error Allocating Buffer\n");
		exit(-1);
	}
	buffer->buffer_size = DYNAMIC_ARRAY_START_SIZE;

	return buffer;
}

void dynamic_array_set(dynamic_array* array, uint64_t index, uint8_t data)
{
	if (index >= array->buffer_size)
	{
		uint64_t new_size = array->buffer_size * DYNAMIC_ARRAY_GROW_RATE;
		while (index >= new_size)
			new_size *= DYNAMIC_ARRAY_GROW_RATE;
		uint8_t* new_buffer = malloc(new_size);
		if (new_buffer == 0)
		{
			printf("Error Allocating Buffer\n");
			exit(-1);
		}
		memcpy(new_buffer, array->buffer, array->buffer_size);

		free(array->buffer);
		array->buffer = new_buffer;
		array->buffer_size = new_size;
	}

	array->buffer[index] = data;
}

uint8_t dynamic_array_get(dynamic_array* array, uint64_t index)
{
	return array->buffer[index];
}

uint8_t* dynamic_array_get_static_copy(dynamic_array* array)
{
	uint8_t* new_buffer = malloc(array->buffer_size);
	if (new_buffer == 0)
	{
		printf("Error Allocating Buffer\n");
		exit(-1);
	}
	memcpy(new_buffer, array->buffer, array->buffer_size);
	return new_buffer;
}

uint64_t dynamic_array_get_size(dynamic_array* array)
{
	return array->buffer_size;
}

void dynamic_array_free(dynamic_array* array)
{
	free(array->buffer);
	free(array);
}

dynamic_array_16* dynamic_array_16_create()
{
	dynamic_array_16* buffer = malloc(sizeof(struct dynamic_array_16));
	if (buffer == 0)
	{
		printf("Error Allocating Buffer\n");
		exit(-1);
	}

	buffer->buffer = malloc(DYNAMIC_ARRAY_START_SIZE * 2);
	if (buffer->buffer == 0)
	{
		free(buffer);
		printf("Error Allocating Buffer\n");
		exit(-1);
	}
	buffer->buffer_size = DYNAMIC_ARRAY_START_SIZE;

	return buffer;
}

void dynamic_array_16_set(dynamic_array_16* array, uint64_t index, uint16_t data)
{
	if (index >= array->buffer_size)
	{
		uint64_t new_size = array->buffer_size * DYNAMIC_ARRAY_GROW_RATE;
		while (index >= new_size)
			new_size *= DYNAMIC_ARRAY_GROW_RATE;
		uint16_t* new_buffer = malloc(new_size * 2);
		if (new_buffer == 0)
		{
			printf("Error Allocating Buffer\n");
			exit(-1);
		}
		memcpy(new_buffer, array->buffer, array->buffer_size * 2);

		free(array->buffer);
		array->buffer = new_buffer;
		array->buffer_size = new_size;
	}

	array->buffer[index] = data;
}

uint16_t dynamic_array_16_get(dynamic_array_16* array, uint64_t index)
{
	return array->buffer[index];
}

uint16_t* dynamic_array_16_get_static_copy(dynamic_array_16* array)
{
	uint16_t* new_buffer = malloc(array->buffer_size * 2);
	if (new_buffer == 0)
	{
		printf("Error Allocating Buffer\n");
		exit(-1);
	}
	memcpy(new_buffer, array->buffer, array->buffer_size * 2);
	return new_buffer;
}

uint64_t dynamic_array_16_get_size(dynamic_array_16* array)
{
	return array->buffer_size;
}

void dynamic_array_16_free(dynamic_array_16* array)
{
	free(array->buffer);
	free(array);
}