#pragma once
#include <stdint.h>

typedef struct file_information {
	char signature[4];
	uint32_t file_offset;
	uint32_t start_file_name_length;
	uint32_t payload_size;
} file_information;