#pragma once
#include <stdint.h>

enum ImageFileFormat {
	IMAGE_UNKNOWN, IMAGE_PNG, IMAGE_JPEG
};

enum ImageFormat {
	IMAGE_RGB, IMAGE_RGBA
};

typedef struct image {
	uint8_t* rawImageData;
	enum ImageFormat format;
	uint32_t width;
	uint32_t height;
} image;

enum JPEG_SEGMENT_TYPE {
	JPEG_SEGMENT_START_OF_IMAGE = 0xD8,
	JPEG_SEGMENT_END_OF_IMAGE = 0xD9,
	JPEG_SEGMENT_JFIF = 0xE0,
	JPEG_SEGMENT_QUANTIZATION_TABLE = 0xdb,
	JPEG_SEGMENT_PROGRESSIVE_DCT = 0xc2,
	JPEG_SEGMENT_HUFFMAN_TABLE = 0xc4,
	JPEG_SEGMENT_START_OF_SCAN = 0xda
};

#pragma pack(1)
typedef struct jfif_section {
	uint16_t marker;
	uint8_t section_length_high;
	uint8_t section_length_low;
	char jfif_string[5];
	uint16_t jfif_version;
	uint8_t pixel_density;
	uint16_t x_density;
	uint16_t y_density;
	uint8_t x_thumbnail_size;
	uint8_t y_thumbnail_size;
	uint8_t thumbnail_data[];
} jfif_section;

typedef struct jpeg_quantization_table_section {
	uint16_t marker;
	uint8_t section_length_high;
	uint8_t section_length_low;
} jpeg_quantization_table_section;

typedef struct jpeg_progressive_dct_frame_component {
	uint8_t c;
	uint8_t h : 4;
	uint8_t v : 4;
	uint8_t tq;
} jpeg_progressive_dct_frame_component;

typedef struct jpeg_progressive_dct_section {
	uint16_t marker;
	uint8_t section_length_high;
	uint8_t section_length_low;

	// 6
	uint8_t p;
	uint16_t y;
	uint16_t x;
	uint8_t nf;

	// 3
	jpeg_progressive_dct_frame_component progressive_dct_frame_components[];
} jpeg_progressive_dct_section;

typedef struct jpeg_huffman_table_section {
	uint16_t marker;
	uint8_t section_length_high;
	uint8_t section_length_low;
} jpeg_huffman_table_section;

typedef struct jpeg_start_of_scan_component {
	uint8_t cs;
	uint8_t td : 4;
	uint8_t ta : 4;
} jpeg_start_of_scan_component;

typedef struct jpeg_start_of_scan_section {
	uint16_t marker;
	uint8_t section_length_high;
	uint8_t section_length_low;

	uint8_t ns;
	jpeg_start_of_scan_component components[];

	// uint8_t ss;
	// uint8_t se;
	// uint8_t ah : 4;
	// uint8_t al : 4;
} jpeg_start_of_scan_section;
#pragma pack()

typedef struct jpeg_image {
	jfif_section* jfif_section;
} jpeg_image;

image* read_image_from_file(char* file_name);
image* read_image_from_data(uint8_t* image_data, uint64_t buffer_size);