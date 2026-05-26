#include "DynamicBuffer.h"
#include "DynamicArray.h"
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
	if ((buffer_size >= 2) && (image_data[0] == 0x42) && (image_data[1] == 0x4D))
		return IMAGE_BMP;
	
	return IMAGE_UNKNOWN;
}


image* read_image_from_data(uint8_t* image_data, uint64_t buffer_size)
{
	switch (getImageFileFormat(image_data, buffer_size))
	{
	case IMAGE_JPEG:
		printf("JPEG Not Supported!\n");
		return 0;
		return read_jpeg_image(image_data, buffer_size);

	case IMAGE_BMP:
		return read_bmp_image(image_data, buffer_size);

	case IMAGE_UNKNOWN:
		printf("Unknown Image Format!\n");
		return 0;
	}
}

void write_image_to_file(image* image, enum ImageFileFormat format, char* file_name)
{
	HANDLE file_handle = CreateFileA(file_name, GENERIC_WRITE, 0, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
	if (file_handle == 0)
	{
		printf("File %s could not be opened!\n", file_name);
		return 0;
	}
	
	dynamic_buffer* out_buffer = dynamic_buffer_create();
	switch (format)
	{
	case IMAGE_BMP:
		write_bmp_image(image, out_buffer);
		break;

	case IMAGE_UNKNOWN:
		printf("Unknown Image Format!\n");
		break;

	default:
		printf("Writing to this Image Format not supported!\n");
		break;
	}

	uint8_t* output_buffer = dynamic_array_get_static_copy(out_buffer);

	DWORD bytesRead = 0;
	BOOL success = WriteFile(file_handle, output_buffer, dynamic_buffer_get_size(out_buffer), &bytesRead, 0);

	if (!success)
		printf("Error writing file!\n");
	if (bytesRead != dynamic_buffer_get_size(out_buffer))
		printf("Not all bytes of image written!\n");

	free(output_buffer);
	dynamic_buffer_free(out_buffer);
	CloseHandle(file_handle);
}

void convert_image_from_bgr_to_argb(image* image)
{
	uint64_t pixels = image->height * image->width;
	uint8_t* new_pixel_buffer = malloc(pixels * 4);
	if (new_pixel_buffer == 0)
	{
		printf("Error Allocating Memory for Pixel Buffer!\n");
		return 0;
	}

	for (int i = 0; i < pixels; i++)
	{
		//memcpy(&(new_pixel_buffer[i * 4]), &(image->rawImageData[i * 3]), 3);
		new_pixel_buffer[i * 4 + 0] = image->rawImageData[i * 3 + 2];
		new_pixel_buffer[i * 4 + 1] = image->rawImageData[i * 3 + 1];
		new_pixel_buffer[i * 4 + 2] = image->rawImageData[i * 3 + 0];
		new_pixel_buffer[i * 4 + 3] = 0xFF;
	}

	free(image->rawImageData);
	image->rawImageData = new_pixel_buffer;
}

void convert_image_to_rgba(image* image)
{
	switch (image->format)
	{
	case IMAGE_BGR:
		convert_image_from_bgr_to_argb(image);
		break;
	default:
		printf("Unsupported Image Format to convert to!\n");
		return;
	}

	image->format = IMAGE_RGBA;
}

void convert_image_to_format(image* image, enum ImageFormat format)
{
	if (image->format == format)
		return;

	switch (format)
	{
	case IMAGE_RGBA:
		convert_image_to_rgba(image);
		break;
	default:
		printf("Unsupported Image Format to convert to!\n");
		return;
	}
}

uint8_t image_get_pixel_per_byte(enum ImageFormat format)
{
	switch (format)
	{
	case IMAGE_ARGB:
	case IMAGE_RGBA:
	case IMAGE_BGRA:
		return 4;
	case IMAGE_RGB:
	case IMAGE_BGR:
		return 3;
	}
}

image* create_empty_image(uint32_t width, uint32_t height, enum ImageFormat format)
{
	image* image = malloc(sizeof(struct image));
	if (image == 0)
	{
		printf("Error allocating memory for the image!\n");
		exit(-1);
	}

	image->format = format;
	image->width = width;
	image->height = height;
	image->rawImageData = malloc(image_get_pixel_per_byte(format) * width * height);
	if (image->rawImageData == 0)
	{
		printf("Error allocating memory for the image!\n");
		exit(-1);
	}
	return image;
}

int main()
{
	image* image = read_image_from_file("font.bmp");
	//convert_image_to_format(image, IMAGE_RGBA);
	write_image_to_file(image, IMAGE_BMP, "output.bmp");
	
	return 0;
}