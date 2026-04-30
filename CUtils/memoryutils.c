#include "cutils.h"
#include "stdio.h"
#include <Windows.h>

void* safe_malloc(uint64_t size)
{
	void* address = malloc(size);
	if (address == 0)
	{
		fprintf(stderr, "Error allocating 0x%llx Bytes\n", size);
		exit(-1);
	}
	return address;
}