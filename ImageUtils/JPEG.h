#pragma once
#include "ImageUtils.h"
#include "DynamicBuffer.h"
#include "DynamicArray.h"


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

typedef struct jpeg_quantization_table {
	uint8_t tq : 4;
	uint8_t pq : 4;
	/*
	union
	{
		uint8_t q_8;
		uint16_t q_16;
	}[64];
	*/
} jpeg_quantization_table;

typedef struct jpeg_quantization_table_section {
	uint16_t marker;
	uint8_t section_length_high;
	uint8_t section_length_low;
} jpeg_quantization_table_section;

typedef struct jpeg_frame_component {
	uint8_t c;
	uint8_t v : 4;
	uint8_t h : 4;
	uint8_t tq;
} jpeg_frame_component;

typedef struct jpeg_frame_section {
	uint16_t marker;
	uint8_t section_length_high;
	uint8_t section_length_low;

	// 6
	uint8_t p;
	uint8_t y_high;
	uint8_t y_low;
	uint8_t x_high;
	uint8_t x_low;
	uint8_t nf;

	// 3
	jpeg_frame_component frame_components[];
} jpeg_frame_section;

typedef struct jpeg_huffman_table_section {
	uint16_t marker;
	uint8_t section_length_high;
	uint8_t section_length_low;
} jpeg_huffman_table_section;

typedef struct jpeg_huffman_table {
	uint8_t th : 4;
	uint8_t tc : 4;
	uint8_t l[16];
	uint8_t v[];
} jpeg_huffman_table;

typedef struct jpeg_usable_huffman_table {
	dynamic_array* HUFFSIZE;
	dynamic_array_16* HUFFCODE;

	dynamic_array* EHUFSI;
	dynamic_array_16* EHUFCO;

	uint8_t last_k;

	uint8_t* huffman_size;
	uint16_t* huffman_codes;

	uint8_t th : 4;
	uint8_t tc : 4;
} jpeg_usable_huffman_table;

typedef struct jpeg_start_of_scan_component {
	uint8_t cs;
	uint8_t ta : 4;
	uint8_t td : 4;
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

typedef struct jpeg_start_of_scan_section_2 {
	uint8_t ss;
	uint8_t se;
	uint8_t al : 4;
	uint8_t ah : 4;
} jpeg_start_of_scan_section_2;
#pragma pack()

typedef struct jpeg_image {
	jfif_section* jfif_section;
} jpeg_image;

typedef struct jpeg_decoder {
	union
	{
		jpeg_usable_huffman_table* huffman_slots[4];
		struct
		{
			jpeg_usable_huffman_table* huffman_slot_0;
			jpeg_usable_huffman_table* huffman_slot_1;
			jpeg_usable_huffman_table* huffman_slot_2;
			jpeg_usable_huffman_table* huffman_slot_3;
		};
	};
	union
	{
		jpeg_quantization_table* quantization_table_slots[4];
		struct
		{
			jpeg_quantization_table* quantization_table_slot_0;
			jpeg_quantization_table* quantization_table_slot_1;
			jpeg_quantization_table* quantization_table_slot_2;
			jpeg_quantization_table* quantization_table_slot_3;
		};
	};
	jpeg_frame_component* frame_components[256];
} jpeg_decoder;