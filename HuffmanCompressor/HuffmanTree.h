#pragma once
#include <stdint.h>

typedef struct huffman_tree_memory_allocation_struct {
	void* memory;
	uint64_t current_max_size;
	uint64_t current_size;
} huffman_tree_memory_allocation_struct;

#define HUFFMAN_TREE_LEAF 0
#define HUFFMAN_TREE_NODE 1

typedef struct huffman_tree_entry {
	uint8_t type : 1;
	uint8_t unsused : 7;
} huffman_tree_entry;

typedef struct huffman_tree_node {
	uint8_t type : 1;
	uint8_t zero_offset_top : 3;
	uint8_t one_offset_top : 3;
	uint8_t unused: 1;
	uint8_t zero_offset;
	uint8_t one_offset;
} huffman_tree_node;


typedef struct huffman_tree_leaf {
	uint8_t type : 1;
	uint8_t unsused : 7;
	uint8_t word;
} huffman_tree_leaf;