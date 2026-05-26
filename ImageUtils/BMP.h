#pragma once
#include "ImageUtils.h"

typedef enum BMP_PIXEL_FORMAT {
	BMP_PIXEL_FORMAT_UNKNOWN, BMP_PIXEL_FORMAT_B8G8R8, BMP_PIXEL_FORMAT_R8G8B8A8, BMP_PIXEL_FORMAT_B8G8R8A8, BMP_PIXEL_FORMAT_A8R8G8B8
};

typedef enum BMP_COMPRESSION {
	BMP_COMPRESSION_RGB = 0,
	BMP_COMPRESSION_BITFIELDS = 3,
	BMP_COMPRESSION_ALPHABITFIELDS = 6
};

#pragma pack(2)
typedef struct bmp_file_header {
	uint16_t signature;
	uint32_t file_size;
	uint16_t reserved_1;
	uint16_t reserved_2;
	uint32_t pixel_array_offset;
} bmp_file_header;
#pragma pack()

typedef struct bmp_dib_header_v5 {
	uint32_t dib_header_size;
	uint32_t image_width;
	uint32_t image_height;
	uint16_t planes;
	uint16_t bits_per_pixel;
	uint32_t compression;
	uint32_t image_size;
	uint32_t x_pixel_per_meter;
	uint32_t y_pixel_per_meter;
	uint32_t colors_in_color_table;
	uint32_t important_color_count;
	uint32_t red_channel_bitmask;
	uint32_t green_channel_bitmask;
	uint32_t blue_channel_bitmask;
	uint32_t alpha_channel_bitmask;
	uint32_t color_space_type;
	uint32_t color_space_endpoints[9];
	uint32_t gamma_for_red_channel;
	uint32_t gamma_for_green_channel;
	uint32_t gamma_for_blue_channel;
	uint32_t intent;
	uint32_t icc_profile_data;
	uint32_t icc_profile_size;
	uint32_t reserved;
} bmp_dib_header_v5;

typedef struct bmp_dib_info_header {
	uint32_t dib_header_size;
	int32_t image_width;
	int32_t image_height;
	uint16_t planes;
	uint16_t bits_per_pixel;
	uint32_t compression;
	uint32_t image_size;
	int32_t x_pixel_per_meter;
	int32_t y_pixel_per_meter;
	uint32_t colors_in_color_table;
	uint32_t important_color_count;
} bmp_dib_info_header;