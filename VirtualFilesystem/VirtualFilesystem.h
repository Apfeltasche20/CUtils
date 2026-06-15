#pragma once
#include <stdint.h>

#define VIRTUAL_FILESYSTEM_TYPE_NORMAL 1
#define VIRTUAL_FILESYSTEM_TYPE_PACKED 2

typedef struct virtual_filesystem {
	uint8_t type;

	void(*read_file)(virtual_filesystem filesystem, char* name);
	void(*create_file)(virtual_filesystem filesystem, char* name);
	void(*list_files)(virtual_filesystem filesystem, char* dir);
} virtual_filesystem;

typedef struct virtual_filesystem_normal {
	virtual_filesystem base;

	char* root;
} virtual_filesystem_normal;

virtual_filesystem* virtual_filesystem_create_from_normal_filesystem(char* root);
virtual_filesystem* virtual_filesystem_create_from_packed_file(char* file_path);
virtual_filesystem* virtual_filesystem_create_from_packed_data(uint8_t* data, uint64_t data_length);