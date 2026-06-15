#pragma once
#include <stdint.h>

typedef struct packed_filesystem_file_header {
	char signature[4];
	uint32_t version;
	uint32_t unused;
	uint32_t offset_to_root_dir;
} packed_filesystem_file_header;

#define PACKED_FILESYSTEM_ENTRY_FILE 1
#define PACKED_FILESYSTEM_ENTRY_DIRECTORY 2

typedef struct packed_filesystem_entry {
	uint16_t type;
	uint16_t flags;
} packed_filesystem_entry;

#define PACKED_FILESYSTEM_FLAG_DIRECTORY_NORMAL 1

typedef struct packed_filesystem_entry_directory {
	uint16_t type; // 2
	uint16_t flags;
	uint32_t child_entries;
	uint32_t unused;
	uint32_t offset_to_data;
	char name[]; // null terminated
} packed_filesystem_entry_directory;

#define PACKED_FILESYSTEM_FLAG_FILE_NORMAL 0b1
#define PACKED_FILESYSTEM_FLAG_FILE_HUFFMAN 0b10

typedef struct packed_filesystem_entry_file {
	uint16_t type; // 1
	uint16_t flags;
	uint32_t compressed_file_size;
	uint32_t uncompressed_file_size;
	uint32_t data_size;
	uint32_t offset_to_data;
	char name[]; // null terminated
} packed_filesystem_entry_file;

void pack_directory(char* directory, char* output_file);
void unpack_directory_data(char* output_directory, uint8_t* data, uint64_t data_length);
void unpack_directory(char* output_directory, char* file);