#include "DynamicBuffer.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DYNAMIC_BUFFER_START_SIZE 8
#define DYNAMIC_BUFFER_GROW_RATE 2

dynamic_buffer* dynamic_buffer_create()
{
	dynamic_buffer* buffer = malloc(sizeof(struct dynamic_buffer));
	if (buffer == 0)
	{
		printf("Error Allocating Buffer\n");
		exit(-1);
	}

	buffer->buffer = malloc(DYNAMIC_BUFFER_START_SIZE);
	if (buffer->buffer == 0)
	{
		free(buffer);
		printf("Error Allocating Buffer\n");
		exit(-1);
	}
	buffer->size = 0;
	buffer->buffer_size = DYNAMIC_BUFFER_START_SIZE;

	return buffer;
}

void dynamic_buffer_add(dynamic_buffer* buffer, uint8_t data)
{
	if (buffer->size >= buffer->buffer_size)
	{
		uint64_t new_size = buffer->buffer_size * DYNAMIC_BUFFER_GROW_RATE;
		uint8_t* new_buffer = malloc(new_size);
		if (new_buffer == 0)
		{
			printf("Error Allocating Buffer\n");
			exit(-1);
		}
		memcpy(new_buffer, buffer->buffer, buffer->buffer_size);

		free(buffer->buffer);
		buffer->buffer = new_buffer;
		buffer->buffer_size = new_size;
	}

	buffer->buffer[buffer->size] = data;
	buffer->size += 1;
}

void dynamic_buffer_add_array(dynamic_buffer* buffer, uint8_t* data, uint64_t data_length)
{
	if ((buffer->size + data_length) >= (buffer->buffer_size))
	{
		uint64_t new_size = buffer->buffer_size * DYNAMIC_BUFFER_GROW_RATE;
		while(new_size <= (buffer->size + data_length))
			new_size *= DYNAMIC_BUFFER_GROW_RATE;
		uint8_t* new_buffer = malloc(new_size);
		if (new_buffer == 0)
		{
			printf("Error Allocating Buffer\n");
			exit(-1);
		}
		memcpy(new_buffer, buffer->buffer, buffer->buffer_size);

		free(buffer->buffer);
		buffer->buffer = new_buffer;
		buffer->buffer_size = new_size;
	}

	memcpy(&(buffer->buffer[buffer->size]), data, data_length);
	buffer->size += data_length;
}

void dynamic_buffer_set_index_8(dynamic_buffer* buffer, uint64_t index, uint8_t data)
{
	if (index >= buffer->buffer_size)
	{
		printf("Index is out of Bounds!\n");
		return;
	}

	buffer->buffer[index] = data;
}

void dynamic_buffer_set_index_16(dynamic_buffer* buffer, uint64_t index, uint16_t data)
{
	if ((index + 1) >= buffer->buffer_size)
	{
		printf("Index is out of Bounds!\n");
		return;
	}

	*(uint16_t*)&(buffer->buffer[index]) = data;
}

void dynamic_buffer_set_index_32(dynamic_buffer* buffer, uint64_t index, uint32_t data)
{
	if ((index + 3) >= buffer->buffer_size)
	{
		printf("Index is out of Bounds!\n");
		return;
	}

	*(uint32_t*)&(buffer->buffer[index]) = data;
}

uint8_t* dynamic_buffer_get_buffer(dynamic_buffer* buffer)
{
	return buffer->buffer;
}

uint8_t* dynamic_buffer_copy_buffer(dynamic_buffer* buffer)
{
	uint8_t* new_buffer = malloc(buffer->size);
	if (new_buffer == 0)
	{
		printf("Error Allocating Buffer\n");
		exit(-1);
	}
	memcpy(new_buffer, buffer->buffer, buffer->size);
	return new_buffer;
}

uint64_t dynamic_buffer_get_size(dynamic_buffer* buffer)
{
	return buffer->size;
}

void dynamic_buffer_free(dynamic_buffer* buffer)
{
	free(buffer->buffer);
	free(buffer);
}