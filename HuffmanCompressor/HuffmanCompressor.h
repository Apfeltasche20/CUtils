#pragma once
#include "HuffmanTree.h"
#include <stdint.h>

uint8_t* huffman_encode_data(uint8_t* input_data, uint64_t input_data_length, huffman_tree_node* root, uint64_t* output_data_size);
