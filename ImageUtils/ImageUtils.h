#pragma once
#include <stdint.h>
#include "DynamicArray.h"
#include "DynamicBuffer.h"
#include "JPEG.h"
#include "Windows.h"

enum ImageFileFormat {
	IMAGE_UNKNOWN, IMAGE_PNG, IMAGE_JPEG, IMAGE_BMP
};

enum ImageFormat {
	IMAGE_RGB, IMAGE_RGBA, IMAGE_BGR
};

typedef struct image {
	uint8_t* rawImageData;
	enum ImageFormat format;
	uint32_t width;
	uint32_t height;

	// extra info
	uint32_t x_resolution;
	uint32_t y_resolution;
} image;

extern image* read_jpeg_image(uint8_t* image_data, uint64_t buffer_size);
extern image* read_bmp_image(uint8_t* image_data, uint64_t buffer_size);
extern void write_bmp_image(image* image, dynamic_buffer* out_buffer);

image* read_image_from_file(char* file_name);
image* read_image_from_data(uint8_t* image_data, uint64_t buffer_size);

void write_image_to_file(image* image, enum ImageFileFormat format, char* file_name);

void convert_image_to_format(image* image, enum ImageFormat format);