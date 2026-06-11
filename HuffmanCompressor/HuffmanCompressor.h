#pragma once
#include "HuffmanTree.h"
#include <stdint.h>

uint8_t* huffman_encode_data(uint8_t* input_data, uint64_t input_data_length, huffman_tree_node** root, uint64_t* output_data_size, huffman_tree_memory_allocation_struct** huffman_tree_memory_allocation_struct, uint8_t* filling_bits_count);
uint8_t* huffman_decode_data(uint8_t* input_data, uint64_t input_data_length, huffman_tree_node* huffman_tree, uint64_t output_data_size);
