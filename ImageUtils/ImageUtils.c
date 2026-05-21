#include "ImageUtils.h"
#include <Windows.h>
#include "stdio.h"

image* read_image_from_file(char* file_name)
{
	image* result_image = 0;
	HANDLE file_handle = CreateFileA(file_name, GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	if (file_handle == 0)
	{
		printf("File %s could not be opened!\n", file_name);
		return 0;
	}

	LARGE_INTEGER file_size;
	BOOL success = GetFileSizeEx(file_handle, &file_size);
	if (!success)
	{
		printf("Error getting file size!\n");
		goto ERROR_CLOSE_HANDLE;
	}

	uint8_t* image_data = malloc(file_size.QuadPart);
	if (image_data == 0)
	{
		printf("Error allocating memory for file reading\n");
		goto ERROR_CLOSE_HANDLE;
	}

	DWORD bytesRead = 0;
	success = ReadFile(file_handle, image_data, file_size.QuadPart, &bytesRead, 0);
	if (!success)
	{
		printf("Error reading file!\n");
		goto ERROR_CLOSE_HANDLE;
	}

	if (bytesRead != file_size.QuadPart)
	{
		printf("Not all bytes of file read!\n");
		goto ERROR_FREE_BUFFER;
	}

	result_image = read_image_from_data(image_data, file_size.QuadPart);

ERROR_FREE_BUFFER:
	free(image_data);
ERROR_CLOSE_HANDLE:
	CloseHandle(file_handle);
	return result_image;
}

enum ImageFileFormat getImageFileFormat(uint8_t* image_data, uint64_t buffer_size)
{
	if ((buffer_size >= 3) && (image_data[0] == 0xff) && (image_data[1] == 0xd8) && (image_data[2] == 0xff))
		return IMAGE_JPEG;
	
	return IMAGE_UNKNOWN;
}

#define CHECK_BUFFER_BOUNDS(error_message) if (current_buffer_position >= buffer_size) {printf(error_message); return 0;}
#define CHECK_BUFFER_LENGTH(length, error_message) if ((current_buffer_position + length) >= buffer_size) {printf(error_message); return 0;}
#define get_section_length(section) ((section->section_length_high << 8) | section->section_length_low)

image* decode_jpeg_image(jpeg_image* jpeg_image)
{
	printf("\nStarting decoding JPEG Image\n");
	printf("\nFinished decoding JPEG Image\n");

	return 0;
}

image* decode_progressive_jpeg_image(jpeg_image* jpeg_image, jpeg_progressive_dct_section* progressive_dct_section, uint64_t current_buffer_position, uint8_t* image_data, uint64_t buffer_size)
{
	CHECK_BUFFER_BOUNDS("Expected Huffman marker, but hit EOF!\n");
	
	if (image_data[current_buffer_position] != 0xFF)
	{
		printf("Expected JPEG marker, but got 0x%x!\n", image_data[current_buffer_position]);
		return 0;
	}

	current_buffer_position++;
	CHECK_BUFFER_BOUNDS("Expected Huffman marker, but hit EOF!\n");

	uint8_t section_type = image_data[current_buffer_position++];
	if (section_type != JPEG_SEGMENT_HUFFMAN_TABLE)
	{
		printf("Expected Huffman marker, but got other marker 0x%x!\n", section_type);
		return 0;
	}

	CHECK_BUFFER_LENGTH(2, "Expected Huffman section size, but hit EOF!\n");

	jpeg_huffman_table_section* huffman_table_section = (jpeg_huffman_table_section*)&(image_data[current_buffer_position - 2]);
	current_buffer_position += get_section_length(huffman_table_section);

	CHECK_BUFFER_BOUNDS("Huffman section exceeded file size!\n")

	if (image_data[current_buffer_position] != 0xFF)
	{
		printf("Expected JPEG marker, but got 0x%x!\n", image_data[current_buffer_position]);
		return 0;
	}

	current_buffer_position++;
	CHECK_BUFFER_BOUNDS("Expected Start of Scan marker, but hit EOF!\n");

	section_type = image_data[current_buffer_position++];
	if (section_type != JPEG_SEGMENT_START_OF_SCAN)
	{
		printf("Expected Start of Scan marker, but got other marker 0x%x!\n", section_type);
		return 0;
	}

	CHECK_BUFFER_LENGTH(2, "Expected Start of Scan section size, but hit EOF!\n");

	jpeg_start_of_scan_section* start_of_scan_section = (jpeg_start_of_scan_section*)&(image_data[current_buffer_position - 2]);
	current_buffer_position += get_section_length(huffman_table_section);

	CHECK_BUFFER_BOUNDS("Start of Scan section exceeded file size!\n");

	uint64_t data_start = current_buffer_position;
	uint64_t data_end = 0;
	
	printf("");
	return 0;
}

image* read_jpeg_image(uint8_t* image_data, uint64_t buffer_size)
{
	//jpeg_image* jpeg_image = malloc(sizeof(struct jpeg_image));
	jpeg_image jpeg_image;
	
	uint64_t current_buffer_position = 0;
	while (1)
	{
		CHECK_BUFFER_BOUNDS("Expected JPEG marker, but hit EOF!\n");

		/*
		if (search_for_next_marker)
		{
			while (1)
			{
				current_buffer_position++;

				CHECK_BUFFER_BOUNDS("Searched for JPEG marker, but hit EOF!\n");

				if (image_data[current_buffer_position] == 0xFF)
				{
					current_buffer_position++;

					CHECK_BUFFER_BOUNDS("Searched for JPEG marker, but hit EOF!\n");
					if (image_data[current_buffer_position] != 0x0)
					{
						current_buffer_position--;
						break;
					}
				}
			}
			search_for_next_marker = 0;
		}
		*/
		if (image_data[current_buffer_position] != 0xFF)
		{
			printf("Expected JPEG marker, but got 0x%x!\n", image_data[current_buffer_position]);
			return 0;
		}

		current_buffer_position++;
		if (current_buffer_position >= buffer_size)
		{
			printf("Expected JPEG marker type, but hit EOF!\n");
			return 0;
		}

		uint8_t section_type = image_data[current_buffer_position++];
		switch (section_type)
		{
		case JPEG_SEGMENT_START_OF_IMAGE:
			break;

		case JPEG_SEGMENT_JFIF:;
			printf("Found JFIF Section!\n");
			CHECK_BUFFER_LENGTH(2, "Expected JPEG section size, but hit EOF!\n")

			jfif_section* jfif_section = (struct jfif_section*)&(image_data[current_buffer_position - 2]);
			jpeg_image.jfif_section = jfif_section;
			current_buffer_position += get_section_length(jfif_section);

			CHECK_BUFFER_BOUNDS("JPEG section exceeded file size!\n")
			break;

		case JPEG_SEGMENT_QUANTIZATION_TABLE:;
			printf("Found Quantization Table Section!\n");
			CHECK_BUFFER_LENGTH(2, "Expected JPEG section size, but hit EOF!\n")

			jpeg_quantization_table_section* quantization_table_section = (jpeg_quantization_table_section*)&(image_data[current_buffer_position - 2]);
			current_buffer_position += get_section_length(quantization_table_section);

			CHECK_BUFFER_BOUNDS("JPEG section exceeded file size!\n")
			break;

		case JPEG_SEGMENT_PROGRESSIVE_DCT:;
			printf("Found Progressive DCT Section!\n");
			CHECK_BUFFER_LENGTH(2, "Expected JPEG section size, but hit EOF!\n")

			jpeg_progressive_dct_section* progressive_dct_section = (jpeg_progressive_dct_section*)&(image_data[current_buffer_position - 2]);
			current_buffer_position += get_section_length(progressive_dct_section);

			CHECK_BUFFER_BOUNDS("JPEG section exceeded file size!\n")

			return decode_progressive_jpeg_image(&jpeg_image, progressive_dct_section, current_buffer_position, image_data, buffer_size);
			break;
		/*
		case JPEG_SEGMENT_HUFFMAN_TABLE:;
			printf("Found Huffman Table Section!\n");
			CHECK_BUFFER_LENGTH(2, "Expected JPEG section size, but hit EOF!\n")

			jpeg_huffman_table_section* huffman_table_section = (jpeg_huffman_table_section*)&(image_data[current_buffer_position - 2]);
			current_buffer_position += get_section_length(huffman_table_section);

			CHECK_BUFFER_BOUNDS("JPEG section exceeded file size!\n")
			break;

		case JPEG_SEGMENT_START_OF_SCAN:;
			printf("Found Start of Scan Section!\n");
			CHECK_BUFFER_LENGTH(2, "Expected JPEG section size, but hit EOF!\n")

			jpeg_start_of_scan_section* start_of_scan_section = (jpeg_start_of_scan_section*)&(image_data[current_buffer_position - 2]);
			current_buffer_position += get_section_length(start_of_scan_section);

			CHECK_BUFFER_BOUNDS("JPEG section exceeded file size!\n")

			search_for_next_marker = 1;
			break;

		case JPEG_SEGMENT_END_OF_IMAGE:;
			printf("Found End of Image Marker!\n");
			if (current_buffer_position != buffer_size)
				printf("WARNING: Data after end of Image!\n");
			return decode_jpeg_image(&jpeg_image);
			*/
		case JPEG_SEGMENT_HUFFMAN_TABLE:
		case JPEG_SEGMENT_START_OF_SCAN:
		case JPEG_SEGMENT_END_OF_IMAGE:
			printf("Expected SOF header or table header, but got other section 0x%x!\n", section_type);
			return 0;
		default:
			printf("Unknown JPEG image marker 0x%x!\n", section_type);
			return 0;
		}
	}
	return 0;
}

image* read_image_from_data(uint8_t* image_data, uint64_t buffer_size)
{
	switch (getImageFileFormat(image_data, buffer_size))
	{
	case IMAGE_JPEG:
		return read_jpeg_image(image_data, buffer_size);

	case IMAGE_UNKNOWN:
		printf("Unknown Image Format!\n");
		return 0;
	}
}

int main()
{
	read_image_from_file("testIMage.jpg");
	
	return 0;
}