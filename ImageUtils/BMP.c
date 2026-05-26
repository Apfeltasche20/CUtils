#include "BMP.h"
#include "DynamicBuffer.h"
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <Windows.h>

#define CHECK_BUFFER_SIZE(length, error_message) if ((length) > buffer_size) {printf(error_message); return 0;}

enum BMP_PIXEL_FORMAT bmp_dib_info_header_get_pixel_format(bmp_dib_info_header* dib_header)
{
	if ((dib_header->compression == BMP_COMPRESSION_RGB) && (dib_header->bits_per_pixel == 24))
		return BMP_PIXEL_FORMAT_B8G8R8;

	return BMP_PIXEL_FORMAT_UNKNOWN;
}

enum BMP_PIXEL_FORMAT bmp_dib_header_v5_get_pixel_format(bmp_dib_header_v5* dib_header)
{
	// BIT MASK ARE IN BIG ENDIAN!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	
	if ((dib_header->compression == BMP_COMPRESSION_RGB) && (dib_header->bits_per_pixel == 24))
	{
		if ((dib_header->red_channel_bitmask == 0x0000FF00) && (dib_header->green_channel_bitmask == 0x00FF0000) && (dib_header->blue_channel_bitmask == 0xFF000000))
			return BMP_PIXEL_FORMAT_B8G8R8;
	}
	else if ((dib_header->compression == BMP_COMPRESSION_BITFIELDS) && (dib_header->bits_per_pixel == 32))
	{
		if ((dib_header->red_channel_bitmask == 0x00FF0000) && (dib_header->green_channel_bitmask == 0x0000FF00) && (dib_header->blue_channel_bitmask == 0x000000FF) && (dib_header->alpha_channel_bitmask == 0xFF000000))
			return BMP_PIXEL_FORMAT_B8G8R8A8;
		else if ((dib_header->red_channel_bitmask == 0x0000FF00) && (dib_header->green_channel_bitmask == 0x00FF0000) && (dib_header->blue_channel_bitmask == 0xFF000000) && (dib_header->alpha_channel_bitmask == 0x000000FF))
			return BMP_PIXEL_FORMAT_A8R8G8B8;
	}

	return BMP_PIXEL_FORMAT_UNKNOWN;
}

uint8_t bmp_read_pixel_data_b8g8r8(image* result_image, uint32_t pixel_data_start, uint8_t* image_data, uint64_t buffer_size)
{
	result_image->format = IMAGE_BGR;
	
	uint64_t pixels_in_on_width = result_image->width * 3;
	uint64_t padding = (4 - (pixels_in_on_width & 0b11)) & 0b11;
	uint64_t complete_image_data_size = (pixels_in_on_width + padding) * result_image->height;
	CHECK_BUFFER_SIZE(pixel_data_start + complete_image_data_size, "Image Pixel Array Out ouf Bounds!\n");

	uint64_t pixels = result_image->height * result_image->width;
	uint8_t* new_pixel_buffer = malloc(pixels * 3);
	if (new_pixel_buffer == 0)
	{
		printf("Error Allocating Memory for Pixel Buffer!\n");
		return 0;
	}
	
	uint8_t* image_data_start = &(image_data[pixel_data_start]);
	for (int i = 0; i < result_image->height; i++)
	{
		memcpy(&(new_pixel_buffer[i * pixels_in_on_width]), &(image_data_start[i * (pixels_in_on_width + padding)]), pixels_in_on_width);
	}
	result_image->rawImageData = new_pixel_buffer;

	return 1;
}

uint8_t bmp_read_pixel_data_b8g8r8a8(image* result_image, uint32_t pixel_data_start, uint8_t* image_data, uint64_t buffer_size)
{
	result_image->format = IMAGE_BGRA;

	uint64_t complete_image_data_size = result_image->width * result_image->height * 4;
	CHECK_BUFFER_SIZE(pixel_data_start + complete_image_data_size, "Image Pixel Array Out ouf Bounds!\n");

	uint8_t* new_pixel_buffer = malloc(complete_image_data_size);
	if (new_pixel_buffer == 0)
	{
		printf("Error Allocating Memory for Pixel Buffer!\n");
		return 0;
	}

	memcpy(new_pixel_buffer, &(image_data[pixel_data_start]), complete_image_data_size);
	result_image->rawImageData = new_pixel_buffer;

	return 1;
}

uint8_t bmp_read_pixel_data_a8r8g8b8(image* result_image, uint32_t pixel_data_start, uint8_t* image_data, uint64_t buffer_size)
{
	result_image->format = IMAGE_ARGB;

	uint64_t complete_image_data_size = result_image->width * result_image->height * 4;
	CHECK_BUFFER_SIZE(pixel_data_start + complete_image_data_size, "Image Pixel Array Out ouf Bounds!\n");

	uint8_t* new_pixel_buffer = malloc(complete_image_data_size);
	if (new_pixel_buffer == 0)
	{
		printf("Error Allocating Memory for Pixel Buffer!\n");
		return 0;
	}

	memcpy(new_pixel_buffer, &(image_data[pixel_data_start]), complete_image_data_size);
	result_image->rawImageData = new_pixel_buffer;

	return 1;
}

uint8_t bmp_read_pixel_data(image* result_image, enum BMP_PIXEL_FORMAT pixel_format, uint32_t pixel_data_start, uint8_t* image_data, uint64_t buffer_size)
{
	switch (pixel_format)
	{
	case BMP_PIXEL_FORMAT_B8G8R8:
		return bmp_read_pixel_data_b8g8r8(result_image, pixel_data_start, image_data, buffer_size);
	
	case BMP_PIXEL_FORMAT_B8G8R8A8:
		return bmp_read_pixel_data_b8g8r8a8(result_image, pixel_data_start, image_data, buffer_size);

	case BMP_PIXEL_FORMAT_A8R8G8B8:
		return bmp_read_pixel_data_a8r8g8b8(result_image, pixel_data_start, image_data, buffer_size);

	default:
		printf("Reading Pixel Format %i not supported!\n", pixel_format);
		return 0;
	}
}

image* read_bmp_dib_header_5_image(uint8_t* image_data, uint64_t buffer_size, bmp_file_header* bmp_file_header, bmp_dib_header_v5* dib_header)
{
	if (dib_header->colors_in_color_table > 0)
	{
		printf("BMP Images with Color Table not implemented!\n");
		return 0;
	}
	if ((dib_header->compression != BMP_COMPRESSION_RGB) && (dib_header->compression != BMP_COMPRESSION_BITFIELDS))
	{
		printf("Compressed BMP Images not implemented!\n");
		return 0;
	}
	
	enum BMP_PIXEL_FORMAT pixel_format = bmp_dib_header_v5_get_pixel_format(dib_header);
	if (pixel_format == BMP_PIXEL_FORMAT_UNKNOWN)
	{
		printf("Unknown BMP DIB Header V5 Pixel Format!\n");
		return 0;
	}

	image* result_image = malloc(sizeof(struct image));
	if (result_image == 0)
	{
		printf("Error Allocting Memory for Image!\n");
		return 0;
	}
	result_image->width = dib_header->image_width;
	result_image->height = dib_header->image_height;
	result_image->x_resolution = dib_header->x_pixel_per_meter;
	result_image->y_resolution = dib_header->y_pixel_per_meter;

	uint8_t success = bmp_read_pixel_data(result_image, pixel_format, bmp_file_header->pixel_array_offset, image_data, buffer_size);
	if (success)
	{
		return result_image;
	}
	else
	{
		free(result_image);
		return 0;
	}
}

image* read_bmp_dib_info_header_image(uint8_t* image_data, uint64_t buffer_size, bmp_file_header* bmp_file_header, bmp_dib_info_header* dib_header)
{
	if (dib_header->colors_in_color_table > 0)
	{
		printf("BMP Images with Color Table not implemented!\n");
		return 0;
	}
	if (dib_header->compression != BMP_COMPRESSION_RGB)
	{
		printf("Compressed BMP Images not implemented!\n");
		return 0;
	}

	enum BMP_PIXEL_FORMAT pixel_format = bmp_dib_info_header_get_pixel_format(dib_header);
	if (pixel_format == BMP_PIXEL_FORMAT_UNKNOWN)
	{
		printf("Unknown BMP DIB Info Header Pixel Format!\n");
		return 0;
	}

	image* result_image = malloc(sizeof(struct image));
	if (result_image == 0)
	{
		printf("Error Allocting Memory for Image!\n");
		return 0;
	}

	result_image->width = dib_header->image_width;
	result_image->height = dib_header->image_height;
	result_image->x_resolution = dib_header->x_pixel_per_meter;
	result_image->y_resolution = dib_header->y_pixel_per_meter;

	uint8_t success = bmp_read_pixel_data(result_image, pixel_format, bmp_file_header->pixel_array_offset, image_data, buffer_size);
	if (success)
	{
		return result_image;
	}
	else
	{
		free(result_image);
		return 0;
	}
}

image* read_bmp_image(uint8_t* image_data, uint64_t buffer_size)
{
	CHECK_BUFFER_SIZE(sizeof(struct bmp_file_header), "Expected BMP File header but hit EOF!\n");
	bmp_file_header* bmp_file_header = (struct bmp_file_header*) image_data;
	if (bmp_file_header->file_size != buffer_size)
		printf("WARNING: Real File Size and BMP Header File not matching!\n");

	CHECK_BUFFER_SIZE(sizeof(struct bmp_file_header) + 4, "Expected BMP DIB header but hit EOF!\n");
	uint32_t dib_header_size = *(uint32_t*)(((uint64_t)image_data) + sizeof(struct bmp_file_header));
	CHECK_BUFFER_SIZE(sizeof(struct bmp_file_header) + dib_header_size, "Expected BMP DIB File header but hit EOF!\n");

	switch (dib_header_size)
	{
		case sizeof(bmp_dib_info_header) :
			return read_bmp_dib_info_header_image(image_data, buffer_size, bmp_file_header, (bmp_dib_info_header*)(((uint64_t)image_data) + sizeof(struct bmp_file_header)));

		case sizeof(bmp_dib_header_v5) :
			return read_bmp_dib_header_5_image(image_data, buffer_size, bmp_file_header, (bmp_dib_header_v5*)(((uint64_t)image_data) + sizeof(struct bmp_file_header)));

	default:
		printf("Unknwon BMP DIB Header Size: 0x%x!\n", dib_header_size);
		return 0;
	}
}

void bmp_convert_image_bgr_to_b8g8r8(image* image, uint8_t* pixel_data)
{
	uint32_t pixels_in_on_width = image->width * 3;
	uint32_t padding = (4 - (pixels_in_on_width & 0b11)) & 0b11;

	for (int i = 0; i < image->height; i++)
	{
		//&(new_pixel_buffer[i * pixels_in_on_width])
		memcpy(&(pixel_data[i * (pixels_in_on_width + padding)]), &(image->rawImageData[i * pixels_in_on_width]), pixels_in_on_width);
		for (int j = 0; j < padding; j++)
		{
			pixel_data[i * (pixels_in_on_width + padding) + j] = 0;
		}
	}
}

void bmp_convert_image_to_b8g8r8_format(image* image, uint16_t* bits_per_pixel, uint8_t** output_pixel_data, uint32_t* output_pixel_data_length, enum BMP_COMPRESSION* compression)
{
	uint32_t pixels_in_on_width = image->width * 3;
	uint32_t padding = (4 - (pixels_in_on_width & 0b11)) & 0b11;
	uint32_t complete_pixel_data_size = (pixels_in_on_width + padding) * image->height;
	uint8_t* pixel_data = malloc(complete_pixel_data_size);
	if (pixel_data == 0)
	{
		printf("Error allocating Pixel Data!\n");
		exit(-1);
	}

	switch (image->format)
	{
	case IMAGE_BGR:;
		bmp_convert_image_bgr_to_b8g8r8(image, pixel_data);
		break;
	default:
		printf("Unsupported Image Format to Convert to BMP B8G8R8!\n");
		break;
	}

	*output_pixel_data = pixel_data;
	*output_pixel_data_length = complete_pixel_data_size;
	*bits_per_pixel = 24;
	*compression = BMP_COMPRESSION_RGB;
}

void bmp_convert_image_rgba_to_r8g8b8a8(image* image, uint8_t* pixel_data)
{
	uint32_t complete_pixel_data_size = image->width * image->height * 4;

	memcpy(pixel_data, image->rawImageData, complete_pixel_data_size);
}

void bmp_convert_image_to_r8g8b8a8_format(image* image, uint16_t* bits_per_pixel, uint8_t** output_pixel_data, uint32_t* output_pixel_data_length, enum BMP_COMPRESSION* compression)
{
	uint32_t complete_pixel_data_size = image->width * image->height * 4;
	uint8_t* pixel_data = malloc(complete_pixel_data_size);
	if (pixel_data == 0)
	{
		printf("Error allocating Pixel Data!\n");
		exit(-1);
	}

	switch (image->format)
	{
	case IMAGE_RGBA:;
		bmp_convert_image_rgba_to_r8g8b8a8(image, pixel_data);
		break;
	default:
		printf("Unsupported Image Format to Convert to BMP R8G8B8A8!\n");
		break;
	}

	*output_pixel_data = pixel_data;
	*output_pixel_data_length = complete_pixel_data_size;
	*bits_per_pixel = 32;
	*compression = BMP_COMPRESSION_BITFIELDS;
}

enum BMP_PIXEL_FORMAT bmp_convert_image_pixels_to_sutable_format(image* image, uint16_t* bits_per_pixel, uint8_t** output_pixel_data, uint32_t* output_pixel_data_length, enum BMP_COMPRESSION* compression, uint32_t* red_bitmask, uint32_t* green_bitmask, uint32_t* blue_bitmask, uint32_t* alpha_bitmask)
{
	switch (image->format)
	{
	case IMAGE_BGR:;
		bmp_convert_image_to_b8g8r8_format(image, bits_per_pixel, output_pixel_data, output_pixel_data_length, compression);
		if (red_bitmask != 0)
			*red_bitmask = 0x00FF0000;
		if (green_bitmask != 0)
			*green_bitmask = 0x0000FF00;
		if (blue_bitmask != 0)
			*blue_bitmask = 0x000000FF;
		if (alpha_bitmask != 0)
			*alpha_bitmask = 0x00000000;
		return BMP_PIXEL_FORMAT_B8G8R8;
	
	case IMAGE_RGBA:
		bmp_convert_image_to_r8g8b8a8_format(image, bits_per_pixel, output_pixel_data, output_pixel_data_length, compression);
		if (red_bitmask != 0)
			*red_bitmask = 0x000000FF;
		if (green_bitmask != 0)
			*green_bitmask = 0x0000FF00;
		if (blue_bitmask != 0)
			*blue_bitmask = 0x00FF0000;
		if (alpha_bitmask != 0)
			*alpha_bitmask = 0xFF000000;
		return BMP_PIXEL_FORMAT_R8G8B8A8;

	default:
		printf("Unsupported Image Format to Write to BMP\n");
		return 0;
	}

}

void write_bmp_dib_info_header(image* image, dynamic_buffer* out_buffer)
{
	bmp_file_header bmp_file_header;
	bmp_dib_info_header bmp_dib_info_header;

	uint16_t bits_per_pixels;
	uint8_t* pixel_data;
	uint32_t pixel_data_length;
	enum BMP_COMPRESSION bmp_compression;
	enum BMP_PIXEL_FORMAT output_bmp_format = bmp_convert_image_pixels_to_sutable_format(image, &bits_per_pixels, &pixel_data, &pixel_data_length, &bmp_compression, 0, 0, 0, 0);

	uint32_t file_size = sizeof(struct bmp_file_header) + sizeof(struct bmp_dib_info_header) + pixel_data_length;

	bmp_file_header.signature = 0x4d42;
	bmp_file_header.file_size = file_size;
	bmp_file_header.reserved_1 = 0;
	bmp_file_header.reserved_2 = 0;
	bmp_file_header.pixel_array_offset = sizeof(struct bmp_file_header) + sizeof(struct bmp_dib_info_header);

	bmp_dib_info_header.dib_header_size = sizeof(struct bmp_dib_info_header);
	bmp_dib_info_header.image_width = image->width;
	bmp_dib_info_header.image_height = image->height;
	bmp_dib_info_header.planes = 1; // ALWAYS 1
	bmp_dib_info_header.bits_per_pixel = bits_per_pixels;
	bmp_dib_info_header.compression = bmp_compression;
	bmp_dib_info_header.image_size = pixel_data_length;
	bmp_dib_info_header.x_pixel_per_meter = image->x_resolution;
	bmp_dib_info_header.y_pixel_per_meter = image->y_resolution;
	bmp_dib_info_header.colors_in_color_table = 0;
	bmp_dib_info_header.important_color_count = 0;

	dynamic_buffer_add_array(out_buffer, &bmp_file_header, sizeof(struct bmp_file_header));
	dynamic_buffer_add_array(out_buffer, &bmp_dib_info_header, sizeof(struct bmp_dib_info_header));
	dynamic_buffer_add_array(out_buffer, pixel_data, pixel_data_length);

	free(pixel_data);
}

void write_bmp_dib_header_v5_header(image* image, dynamic_buffer* out_buffer)
{
	bmp_file_header bmp_file_header;
	bmp_dib_header_v5 bmp_dib_info_header;

	uint16_t bits_per_pixels;
	uint8_t* pixel_data;
	uint32_t pixel_data_length;
	enum BMP_COMPRESSION bmp_compression;
	uint32_t red_bitmask;
	uint32_t green_bitmask;
	uint32_t blue_bitmask;
	uint32_t alpha_bitmask;
	enum BMP_PIXEL_FORMAT output_bmp_format = bmp_convert_image_pixels_to_sutable_format(image, &bits_per_pixels, &pixel_data, &pixel_data_length, &bmp_compression, &red_bitmask, &green_bitmask, &blue_bitmask, &alpha_bitmask);

	uint32_t file_size = sizeof(struct bmp_file_header) + sizeof(struct bmp_dib_header_v5) + pixel_data_length;

	bmp_file_header.signature = 0x4d42;
	bmp_file_header.file_size = file_size;
	bmp_file_header.reserved_1 = 0;
	bmp_file_header.reserved_2 = 0;
	bmp_file_header.pixel_array_offset = sizeof(struct bmp_file_header) + sizeof(struct bmp_dib_header_v5);

	bmp_dib_info_header.dib_header_size = sizeof(struct bmp_dib_header_v5);
	bmp_dib_info_header.image_width = image->width;
	bmp_dib_info_header.image_height = image->height;
	bmp_dib_info_header.planes = 1; // ALWAYS 1
	bmp_dib_info_header.bits_per_pixel = bits_per_pixels;
	bmp_dib_info_header.compression = bmp_compression;
	bmp_dib_info_header.image_size = pixel_data_length;
	bmp_dib_info_header.x_pixel_per_meter = image->x_resolution;
	bmp_dib_info_header.y_pixel_per_meter = image->y_resolution;
	bmp_dib_info_header.colors_in_color_table = 0;
	bmp_dib_info_header.important_color_count = 0;
	bmp_dib_info_header.red_channel_bitmask = red_bitmask;
	bmp_dib_info_header.green_channel_bitmask = green_bitmask;
	bmp_dib_info_header.blue_channel_bitmask = blue_bitmask;
	bmp_dib_info_header.alpha_channel_bitmask = alpha_bitmask;
	bmp_dib_info_header.color_space_type = 'B' | ('G' << 8) | ('R' << 16) | ('s' << 24);
	for(int i = 0;i<9;i++)
		bmp_dib_info_header.color_space_endpoints[i] = 0;
	bmp_dib_info_header.gamma_for_red_channel = 0;
	bmp_dib_info_header.gamma_for_green_channel = 0;
	bmp_dib_info_header.gamma_for_blue_channel = 0;
	bmp_dib_info_header.intent = 0;
	bmp_dib_info_header.icc_profile_data = 0;
	bmp_dib_info_header.icc_profile_size = 0;
	bmp_dib_info_header.reserved = 0;

	dynamic_buffer_add_array(out_buffer, &bmp_file_header, sizeof(struct bmp_file_header));
	dynamic_buffer_add_array(out_buffer, &bmp_dib_info_header, sizeof(struct bmp_dib_header_v5));
	dynamic_buffer_add_array(out_buffer, pixel_data, pixel_data_length);

	free(pixel_data);
}

void write_bmp_image(image* image, dynamic_buffer* out_buffer)
{
	switch (image->format)
	{
	case IMAGE_BGR:;
		write_bmp_dib_info_header(image, out_buffer);
		//write_bmp_dib_header_v5_header(image, out_buffer);
		break;
	case IMAGE_RGBA:
		write_bmp_dib_header_v5_header(image, out_buffer);
		break;
	default:
		printf("Unsupported Image Format to Write to BMP\n");
		return 0;
	}
}