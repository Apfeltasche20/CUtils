#include "HuffmanCompressor.h"
#include "HuffmanTree.h"
#include <DynamicBuffer.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <Windows.h>

typedef struct huffman_bit_stream {
	uint8_t* data;
	uint64_t current_byte_pos;
	uint8_t current_bit_pos;
} huffman_bit_stream;

void* huffman_malloc(huffman_tree_memory_allocation_struct* memory_struct, uint64_t size)
{
	if ((memory_struct->current_size + size) > memory_struct->current_max_size)
	{
		printf("Your math was wrong!\n");
		exit(-1);
	}

	void* memory = (void*)&(((char*)memory_struct->memory)[memory_struct->current_max_size - (memory_struct->current_size + size)]);
	memory_struct->current_size += size;
	
	return memory;
}

huffman_tree_node* huffman_pop_from_priority_queue(uint64_t* priorities, huffman_tree_node** tree_nodes, uint64_t* out_priority)
{
	huffman_tree_node* return_value = tree_nodes[0];
	*out_priority = priorities[0];

	for (int i = 1; i < 256; i++)
	{
		priorities[i - 1] = priorities[i];
		tree_nodes[i - 1] = tree_nodes[i];
	}

	priorities[255] = 0xFFFFFFFFFFFFFFFF;
	tree_nodes[255] = 0;

	return return_value;
}

void huffman_insert_branch_into_priority_queue(uint64_t* priorities, huffman_tree_node** tree_nodes, uint64_t priority, huffman_tree_node* branch)
{
	for (int i = 0; i < 256; i++)
	{
		if (priorities[i] > priority) // Found Slot
		{
			uint64_t last_priority_entry = priorities[i];
			huffman_tree_node* last_tree_entry = tree_nodes[i];

			priorities[i] = priority;
			tree_nodes[i] = branch;

			// Move rest back in array
			for (int n = (i + 1); n < 256; n++)
			{
				uint64_t temp_priority_entry = priorities[n];
				huffman_tree_node* temp_tree_entry = tree_nodes[n];

				priorities[n] = last_priority_entry;
				tree_nodes[n] = last_tree_entry;

				last_priority_entry = temp_priority_entry;
				last_tree_entry = temp_tree_entry;
			}

			return;
		}
	}
}

void huffman_insert_leaf_into_priority_queue(uint64_t* priorities, huffman_tree_node** tree_nodes, uint64_t priority, uint8_t value, huffman_tree_memory_allocation_struct* memory_allocation_struct)
{
	for (int i = 0; i < 256; i++)
	{
		if (priorities[i] > priority) // Found Slot
		{
			uint64_t last_priority_entry = priorities[i];
			huffman_tree_node* last_tree_entry = tree_nodes[i];

			huffman_tree_leaf* new_leaf = huffman_malloc(memory_allocation_struct, sizeof(struct huffman_tree_leaf));
			new_leaf->type = HUFFMAN_TREE_LEAF;
			new_leaf->word = value;

			priorities[i] = priority;
			tree_nodes[i] = new_leaf;

			// Move rest back in array
			for (int n = (i + 1); n < 256; n++)
			{
				uint64_t temp_priority_entry = priorities[n];
				huffman_tree_node* temp_tree_entry = tree_nodes[n];

				priorities[n] = last_priority_entry;
				tree_nodes[n] = last_tree_entry;

				last_priority_entry = temp_priority_entry;
				last_tree_entry = temp_tree_entry;
			}

			return;
		}
	}
}

void huffman_build_encode_lookup(uint64_t* lookup, uint64_t* length, huffman_tree_node* current_node, uint64_t current_codeword, uint64_t current_length)
{
	uint16_t zero_offset = ((((uint16_t)current_node->zero_offset) | (((uint16_t)current_node->zero_offset_top) << 8)));
	uint16_t one_offset = ((((uint16_t)current_node->one_offset) | (((uint16_t)current_node->one_offset_top) << 8)));
	
	huffman_tree_entry* left = (huffman_tree_entry*)(((uint64_t)current_node) + zero_offset);
	huffman_tree_entry* right = (huffman_tree_entry*)(((uint64_t)current_node) + one_offset);

	if (left->type == HUFFMAN_TREE_NODE)
		huffman_build_encode_lookup(lookup, length, left, (current_codeword << 1), current_length + 1);
	else
	{
		huffman_tree_leaf* leaf = (huffman_tree_leaf*) left;
		lookup[leaf->word] = (current_codeword << 1);
		length[leaf->word] = current_length + 1;
	}

	if (right->type == HUFFMAN_TREE_NODE)
		huffman_build_encode_lookup(lookup, length, right, (current_codeword << 1) | 1, current_length + 1);
	else
	{
		huffman_tree_leaf* leaf = (huffman_tree_leaf*)right;
		lookup[leaf->word] = (current_codeword << 1) | 1;
		length[leaf->word] = current_length + 1;
	}
}

uint8_t* huffman_encode_data(uint8_t* input_data, uint64_t input_data_length, huffman_tree_node** root, uint64_t* output_data_size, huffman_tree_memory_allocation_struct** huffman_tree_memory_allocation_struct, uint8_t* filling_bits_count)
{
	struct huffman_tree_memory_allocation_struct* memory_allocation_struct = malloc(sizeof(struct huffman_tree_memory_allocation_struct));
	if (memory_allocation_struct == 0)
	{
		printf("Error Allocating Memory for Huffman Tree!\n");
		exit(-1);
	}

	/*
		The Huffman Tree should have never more than 255 Nodes and 256 Leafes
		As each Node is 3 Bytes and Leaf 2 Bytes the Tree should never be bigger than 255 * 3 + 256 * 2 = 1277 Bytes
	*/
	memory_allocation_struct->memory = malloc(1277);
	memory_allocation_struct->current_max_size = 1277;
	memory_allocation_struct->current_size = 0;

	if (memory_allocation_struct->memory == 0)
	{
		printf("Error Allocating Memory for Huffman Tree!\n");
		exit(-1);
	}

	uint64_t occurences[256];
	for (int i = 0; i < 256; i++)
		occurences[i] = 0;

	/*
		Count Occurences of each word
	*/
	for (int i = 0; i < input_data_length; i++)
	{
		occurences[input_data[i]]++;
	}

	/*
		Initialize Priority Queue
	*/
	uint64_t priorities[256];
	huffman_tree_entry* tree_nodes[256];

	for (int i = 0; i < 256; i++)
	{
		priorities[i] = 0xFFFFFFFFFFFFFFFF;
		tree_nodes[i] = 0;
	}

	for (int i = 0; i < 256; i++)
	{
		if (occurences[i] > 0)
			huffman_insert_leaf_into_priority_queue(priorities, tree_nodes, occurences[i], i, memory_allocation_struct);
	}

	/*
		Build the Huffman Tree
	*/

	while ((tree_nodes[0] != 0) && (tree_nodes[1] != 0)) // 2 or more entries in Queue
	{
		uint64_t first_priority;
		uint64_t second_priority;

		huffman_tree_entry* first_tree_entry = huffman_pop_from_priority_queue(priorities, tree_nodes, &first_priority);
		huffman_tree_entry* second_tree_entry = huffman_pop_from_priority_queue(priorities, tree_nodes, &second_priority);

		uint64_t new_priority = first_priority + second_priority;

		huffman_tree_node* new_node = huffman_malloc(memory_allocation_struct, sizeof(struct huffman_tree_node));
		new_node->type = HUFFMAN_TREE_NODE;
		/*
			Offset sould always be positive, because the new_node is the currently biggest address
		*/
		uint16_t zero_offset = ((uint16_t)(((uint64_t)first_tree_entry) - ((uint64_t)new_node)));
		new_node->zero_offset = zero_offset & 0xFF;
		new_node->zero_offset_top = (zero_offset >> 8) & 0b111;

		uint16_t one_offset = ((uint16_t)(((uint64_t)second_tree_entry) - ((uint64_t)new_node)));
		new_node->one_offset = one_offset & 0xFF;
		new_node->one_offset_top = (one_offset >> 8) & 0b111;
		// new_node->one_offset = (uint16_t)(((uint64_t)new_node) - ((uint64_t)second_tree_entry));

		huffman_insert_branch_into_priority_queue(priorities, tree_nodes, new_priority, new_node);
	}

	*root = tree_nodes[0];
	*huffman_tree_memory_allocation_struct = memory_allocation_struct;

	/*
		Build Encoding Lookup table
	*/
	uint64_t encode_length[256];
	uint64_t encode_huffman_lookup[256];

	huffman_build_encode_lookup(encode_huffman_lookup, encode_length, *root, 0, 0);

	/*
	for (int i = 0; i < 256; i++)
	{
		printf("%i: %lli, %llx\n", i, encode_length[i], encode_huffman_lookup[i]);
	}
	*/

	/*
		Encode Data
	*/

	dynamic_buffer* out_buffer = dynamic_buffer_create();
	uint8_t current_working_byte = 0;
	uint8_t current_bit_position = 0;

	for (int i = 0; i < input_data_length; i++)
	{
		uint8_t current_data = input_data[i];
		uint64_t code_word_length = encode_length[current_data];
		uint64_t code_word = encode_huffman_lookup[current_data];

		for (int n = 0; n < code_word_length; n++)
		{
			uint8_t current_bit = (code_word >> (code_word_length - 1 - n)) & 1;

			/*
				Append Bit to Output Buffer
			*/
			current_working_byte |= (current_bit << current_bit_position);
			current_bit_position++;
			if (current_bit_position >= 8)
			{
				dynamic_buffer_add(out_buffer, current_working_byte);
				current_bit_position = 0;
				current_working_byte = 0;
			}
		}
	}

	if (current_bit_position != 0)
	{
		*filling_bits_count = 8 - current_bit_position;
		dynamic_buffer_add(out_buffer, current_working_byte);
	}
	else
	{
		*filling_bits_count = 0;
	}

	*output_data_size = dynamic_buffer_get_size(out_buffer);
	char* encoded_data = dynamic_buffer_copy_buffer(out_buffer);
	dynamic_buffer_free(out_buffer);

	return encoded_data;
}

uint8_t huffman_get_next_bit(huffman_bit_stream* bit_stream)
{
	uint8_t bit = ((bit_stream->data[bit_stream->current_byte_pos]) >> bit_stream->current_bit_pos) & 1;

	bit_stream->current_bit_pos++;
	if (bit_stream->current_bit_pos >= 8)
	{
		bit_stream->current_bit_pos = 0;
		bit_stream->current_byte_pos++;
	}

	return bit;
}

uint8_t huffman_get_next_decoded_byte(huffman_bit_stream* bit_stream, huffman_tree_node* huffman_tree)
{
	huffman_tree_node* current_node = huffman_tree;
	
	while (1)
	{
		uint8_t next_bit = huffman_get_next_bit(bit_stream);
		
		if (next_bit == 1)
		{
			uint16_t one_offset = ((((uint16_t)current_node->one_offset) | (((uint16_t)current_node->one_offset_top) << 8)));
			huffman_tree_entry* right = (huffman_tree_entry*)(((uint64_t)current_node) + one_offset);
			if (right->type == HUFFMAN_TREE_LEAF)
			{
				return ((huffman_tree_leaf*)right)->word;
			}
			else
			{
				current_node = right;
			}
		}
		else
		{
			uint16_t zero_offset = ((((uint16_t)current_node->zero_offset) | (((uint16_t)current_node->zero_offset_top) << 8)));
			huffman_tree_entry* left = (huffman_tree_entry*)(((uint64_t)current_node) + zero_offset);
			if (left->type == HUFFMAN_TREE_LEAF)
			{
				return ((huffman_tree_leaf*)left)->word;
			}
			else
			{
				current_node = left;
			}
		}
	}
}

uint8_t* huffman_decode_data(uint8_t* input_data, uint64_t input_data_length, huffman_tree_node* huffman_tree, uint64_t output_data_size)
{
	uint8_t* output_buffer = malloc(output_data_size);
	if (output_buffer == 0)
	{
		printf("Error Allocating Memory for Output Buffer!\n");
		exit(-1);
	}

	huffman_bit_stream huffman_bit_stream;
	huffman_bit_stream.data = input_data;
	huffman_bit_stream.current_bit_pos = 0;
	huffman_bit_stream.current_byte_pos = 0;

	for (int i = 0; i < output_data_size; i++)
	{
		output_buffer[i] = huffman_get_next_decoded_byte(&huffman_bit_stream, huffman_tree);
	}

	return output_buffer;
}

uint8_t* read_file(char* file, uint64_t* size)
{
	HANDLE file_handle = CreateFileA(file, GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	if (file_handle == -1)
	{
		printf("File %s could not be opened!\n", file);
		return 0;
	}

	LARGE_INTEGER file_size;
	BOOL success = GetFileSizeEx(file_handle, &file_size);
	if (!success)
	{
		printf("Error getting file size!\n");
		goto ERROR_CLOSE_HANDLE;
	}

	uint8_t* file_data = malloc(file_size.QuadPart);
	if (file_data == 0)
	{
		printf("Error allocating memory for file reading\n");
		goto ERROR_CLOSE_HANDLE;
	}

	DWORD bytesRead = 0;
	success = ReadFile(file_handle, file_data, file_size.QuadPart, &bytesRead, 0);
	if (!success)
	{
		printf("Error reading file!\n");
		goto ERROR_FREE_BUFFER;
	}

	if (bytesRead != file_size.QuadPart)
	{
		printf("Not all bytes of file read!\n");
		goto ERROR_FREE_BUFFER;
	}
	
	*size = file_size.QuadPart;
	CloseHandle(file_handle);
	return file_data;

ERROR_FREE_BUFFER:
	free(file_data);
ERROR_CLOSE_HANDLE:
	CloseHandle(file_handle);
	return 0;
}

/*
int main()
{
	uint64_t size = 0;
	uint8_t* data = read_file("apfeltaschen.bmp", &size);

	huffman_tree_memory_allocation_struct* huffman_tree_memory_allocation_struct;
	huffman_tree_node* huffman_tree;

	uint64_t output_data_size;
	uint8_t filling_bits_count;
	uint8_t* encoded_data = huffman_encode_data(data, size, &huffman_tree, &output_data_size, &huffman_tree_memory_allocation_struct, &filling_bits_count);

	printf("Input Data Size: 0x%llx\nOutput Data Size: 0x%llx, Huffman Table Size: 0x%llx\nComplete Output Size: 0x%llx\nCompression: %f%%\n", size, output_data_size, huffman_tree_memory_allocation_struct->current_size, output_data_size + huffman_tree_memory_allocation_struct->current_size, (1.0 - ((output_data_size + huffman_tree_memory_allocation_struct->current_size) / (float) size)) * 100);

	return;
	uint8_t* decoded_data = huffman_decode_data(encoded_data, output_data_size, huffman_tree, size);

	uint8_t correct = 1;
	for (int i = 0; i < size; i++)
	{
		if (data[i] != decoded_data[i])
		{
			printf("Data not same after Encoding and Decoding at offset 0x%x!\n", i);
			correct = 0;
		}
	}

	if (correct)
	{
		printf("Encoding and decoding success!\n");
	}

	return 0;
}
//*/
