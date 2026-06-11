#pragma once
#include <stdint.h>

typedef struct huffman_tree_memory_allocation_struct {
	void* memory;
	uint64_t current_max_size;
	uint64_t current_size;
} huffman_tree_memory_allocation_struct;

enum huffman_tree_entry_type {
	HUFFMAN_TREE_LEAF, HUFFMAN_TREE_NODE
};

typedef struct huffman_tree_entry {
	enum huffman_tree_entry_type type;
} huffman_tree_entry;

typedef struct huffman_tree_node {
	enum huffman_tree_entry_type type;
	uint16_t zero_offset;
	uint16_t one_offset;
} huffman_tree_node;

typedef struct huffman_tree_leaf {
	enum huffman_tree_entry_type type;
	uint8_t word;
} huffman_tree_leaf;