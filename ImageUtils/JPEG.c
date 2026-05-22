#include "JPEG.h"
#include "DynamicBuffer.h"
#include "DynamicArray.h"

#define CHECK_BUFFER_BOUNDS(error_message) if (current_buffer_position >= buffer_size) {printf(error_message); return 0;}
#define CHECK_BUFFER_LENGTH(length, error_message) if ((current_buffer_position + length) >= buffer_size) {printf(error_message); return 0;}
#define get_section_length(section) ((section->section_length_high << 8) | section->section_length_low)

uint8_t* jpeg_decode_huffman_table(jpeg_huffman_table* huffman_table, uint8_t* data, uint64_t data_length)
{
	dynamic_buffer* buffer = dynamic_buffer_create();

	for (int i = 0; i < data_length; i++)
	{
		uint8_t current = data[data_length];


	}

	dynamic_buffer_free(buffer);
}

void decode_progressive_jpeg_image_scan(jpeg_image* jpeg_image, jpeg_decoder* jpeg_decoder, jpeg_start_of_scan_section* scan_header, uint8_t* encoded_data, uint64_t encoded_data_length)
{
	printf("Decoding Scan Data\n");
	jpeg_start_of_scan_section_2* scan_header_2 = (jpeg_start_of_scan_section_2*)(((uint64_t)scan_header) + 5 + (2 * scan_header->ns));
	for (int i = 0; i < scan_header->ns; i++)
	{
		printf("- Scan Component\n");
		uint8_t cs = scan_header->components[i].cs;
		uint8_t td = scan_header->components[i].td;
		uint8_t ta = scan_header->components[i].ta;

		jpeg_frame_component* image_frame = jpeg_decoder->frame_components[cs];

		jpeg_usable_huffman_table* dc_huffman_table = jpeg_decoder->huffman_slots[td];
		jpeg_usable_huffman_table* ac_huffman_table = jpeg_decoder->huffman_slots[ta];
		uint8_t use_dc_table = (dc_huffman_table != 0) && (dc_huffman_table->tc == 0);
		uint8_t use_ac_table = (ac_huffman_table != 0) && (ac_huffman_table->tc == 1);

		printf("\t- Image Frame ID %i\n", cs);
		printf("\t- DC Index       %i\n", td);
		printf("\t- AC Index       %i\n", ta);

		if (use_dc_table && use_ac_table)
		{
			printf("DC or AC table, thats the question!\n");
			continue;
		}
		else if (use_dc_table)
		{
			uint8_t decoded_data = jpeg_decode_huffman_table(dc_huffman_table, encoded_data, encoded_data_length);

			printf("\t- Use DC Table in index %i\n", td);
		}
		else if (use_ac_table)
		{
			printf("\t- Use AC Table in index %i\n", ta);
		}
		else
		{
			printf("\t- No possible DC or AC table found!\n");
			continue;
		}
	}
}

uint8_t* jpeg_get_decoded_data(uint64_t current_buffer_position, uint8_t* image_data, uint64_t buffer_size, uint64_t* output_buffer_size, uint64_t* new_buffer_position)
{
	dynamic_buffer* buffer = dynamic_buffer_create();

	while (1)
	{
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
			else
			{
				dynamic_buffer_add(buffer, 0xFF);
			}
		}
		else
		{
			dynamic_buffer_add(buffer, image_data[current_buffer_position]);
		}

		current_buffer_position++;
	}

	uint8_t* data = dynamic_buffer_copy_buffer(buffer);
	*output_buffer_size = dynamic_buffer_get_size(buffer);
	*new_buffer_position = current_buffer_position;
	dynamic_buffer_free(buffer);
	return data;
}

void generate_size_table(jpeg_huffman_table* input_huffman_table, jpeg_usable_huffman_table* ouput_huffman_table)
{
	uint8_t K = 0;
	uint8_t I = 1;
	uint8_t J = 1;

	uint8_t* BITS = input_huffman_table->l;

	while (!(I > 16))
	{
		while (!(J > BITS[I]))
		{
			dynamic_array_set(ouput_huffman_table->HUFFSIZE, K, I);
			K += 1;
			J += 1;
		}

		I += 1;
		J = 1;
	}

	ouput_huffman_table->last_k = K;
	dynamic_array_set(ouput_huffman_table->HUFFSIZE, K, 0);
}

void generate_code_table(jpeg_huffman_table* input_huffman_table, jpeg_usable_huffman_table* ouput_huffman_table)
{
	uint8_t K = 0;
	uint16_t CODE = 0;
	uint8_t SI = dynamic_array_get(ouput_huffman_table->HUFFSIZE, 0);

	while (dynamic_array_get(ouput_huffman_table->HUFFSIZE, K) == SI)
	{
		while (dynamic_array_get(ouput_huffman_table->HUFFSIZE, K) == SI)
		{
			dynamic_array_16_set(ouput_huffman_table->HUFFCODE, K, CODE);
			//ouput_huffman_table->HUFFCODE[K] = CODE;
			CODE += 1;
			K += 1;
		}

		if (dynamic_array_get(ouput_huffman_table->HUFFSIZE, K) == 0)
			break;

		do
		{
			CODE = CODE << 1;
			SI += 1;
		} while (dynamic_array_get(ouput_huffman_table->HUFFSIZE, K) != SI);
	}
}

void order_codes(jpeg_huffman_table* input_huffman_table, jpeg_usable_huffman_table* ouput_huffman_table)
{
	uint8_t K = 0;
	do
	{
		uint8_t I = input_huffman_table->v[K];
		uint16_t huffman_code = dynamic_array_16_get(ouput_huffman_table->HUFFCODE, K);
		uint8_t huffman_length = dynamic_array_get(ouput_huffman_table->HUFFSIZE, K);
		dynamic_array_16_set(ouput_huffman_table->EHUFCO, I, huffman_code);
		dynamic_array_set(ouput_huffman_table->EHUFSI, I, huffman_length);
		K += 1;

		printf("Setting Huffman Code Index %i to: Length: %i, Codeword: 0x%x\n", I, huffman_length, huffman_code);
	} while (K < ouput_huffman_table->last_k);
}

jpeg_usable_huffman_table* jpeg_load_huffman_table(jpeg_huffman_table* input_huffman_table)
{
	jpeg_usable_huffman_table* ouput_huffman_table = malloc(sizeof(struct jpeg_usable_huffman_table));
	if (ouput_huffman_table == 0)
	{
		printf("Error allocating Huffman Table!\n");
		exit(-1);
	}

	ouput_huffman_table->HUFFSIZE = dynamic_array_create();
	ouput_huffman_table->HUFFCODE = dynamic_array_16_create();
	ouput_huffman_table->EHUFSI = dynamic_array_create();
	ouput_huffman_table->EHUFCO = dynamic_array_16_create();

	generate_size_table(input_huffman_table, ouput_huffman_table);
	generate_code_table(input_huffman_table, ouput_huffman_table);
	order_codes(input_huffman_table, ouput_huffman_table);

	ouput_huffman_table->huffman_size = dynamic_array_get_static_copy(ouput_huffman_table->EHUFSI);
	ouput_huffman_table->huffman_codes = dynamic_array_16_get_static_copy(ouput_huffman_table->EHUFCO);
	ouput_huffman_table->tc = input_huffman_table->tc;
	ouput_huffman_table->th = input_huffman_table->th;

	dynamic_array_free(ouput_huffman_table->HUFFCODE);
	dynamic_array_16_free(ouput_huffman_table->HUFFSIZE);
	dynamic_array_free(ouput_huffman_table->EHUFCO);
	dynamic_array_16_free(ouput_huffman_table->EHUFSI);
	ouput_huffman_table->HUFFCODE = 0;
	ouput_huffman_table->HUFFSIZE = 0;

	//for (int i = 0; ouput_huffman_table->huffman_size[i] != 0; i++)
	//{
	//	printf("Length: %i, Value 0x%x\n", ouput_huffman_table->huffman_size[i], ouput_huffman_table->huffman_codes[i]);
	//}

	return ouput_huffman_table;
}

image* decode_progressive_jpeg_image(jpeg_decoder* jpeg_decoder, jpeg_image* jpeg_image, jpeg_frame_section* progressive_dct_section, uint64_t current_buffer_position, uint8_t* image_data, uint64_t buffer_size)
{
	printf("Number of Lines: %i\n", (progressive_dct_section->y_high << 8) | progressive_dct_section->y_low);
	printf("Number of Samples per line: %i\n", (progressive_dct_section->x_high << 8) | progressive_dct_section->x_low);
	for (int i = 0; i < progressive_dct_section->nf; i++)
	{
		int c = progressive_dct_section->frame_components[i].c;
		int h = progressive_dct_section->frame_components[i].h;
		int v = progressive_dct_section->frame_components[i].v;
		int tq = progressive_dct_section->frame_components[i].tq;

		jpeg_decoder->frame_components[c] = &(progressive_dct_section->frame_components);
		printf("Loading Frame Component with Id %i\n", c);
		printf("- Horizontal Sampling Factor: %i\n", h);
		printf("- Vertical Sampling Factor: %i\n", v);
		printf("- Quantization Table Slot: %i\n", tq);
	}

	while (1)
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
		switch (section_type)
		{
		case JPEG_SEGMENT_HUFFMAN_TABLE:
			CHECK_BUFFER_LENGTH(2, "Expected Huffman section size, but hit EOF!\n");

			jpeg_huffman_table_section* huffman_table_section = (jpeg_huffman_table_section*)&(image_data[current_buffer_position - 2]);
			uint64_t section_length = get_section_length(huffman_table_section);
			uint64_t current_section_length = current_buffer_position + 2;
			current_buffer_position += section_length;
			CHECK_BUFFER_BOUNDS("Huffman section exceeded file size!\n");

			while (current_section_length < current_buffer_position)
			{
				jpeg_huffman_table* huffman_table = (jpeg_huffman_table*)&(image_data[current_section_length]);

				jpeg_load_huffman_table(huffman_table);
				jpeg_decoder->huffman_slots[huffman_table->th] = jpeg_load_huffman_table(huffman_table);
				printf("Loading Huffman Table with Class %i into Slot %i\n", huffman_table->tc, huffman_table->th);

				uint64_t m = 0;
				for (int i = 0; i < 16; i++)
				{
					m += huffman_table->l[i];
				}
				current_section_length += 17 + m;
			}

			if (current_section_length != current_buffer_position)
				printf("WARNING: HUffman Tables not correctly loaded!\n");

			break;

		case JPEG_SEGMENT_START_OF_SCAN:
			CHECK_BUFFER_LENGTH(2, "Expected Start of Scan section size, but hit EOF!\n");

			jpeg_start_of_scan_section* start_of_scan_section = (jpeg_start_of_scan_section*)&(image_data[current_buffer_position - 2]);
			current_buffer_position += get_section_length(start_of_scan_section);

			CHECK_BUFFER_BOUNDS("Start of Scan section exceeded file size!\n");
			uint64_t scan_data_size = 0;
			uint8_t* scan_data = jpeg_get_decoded_data(current_buffer_position, image_data, buffer_size, &scan_data_size, &current_buffer_position);
			decode_progressive_jpeg_image_scan(jpeg_image, jpeg_decoder, start_of_scan_section, scan_data, scan_data_size);
			free(scan_data);
			break;
		case JPEG_SEGMENT_END_OF_IMAGE:
			printf("Found End of Image Marker!\n");
			if (current_buffer_position != buffer_size)
				printf("WARNING: Data after end of Image!\n");
			return 0;
		}
	}

	return 0;
}

image* read_jpeg_image(uint8_t* image_data, uint64_t buffer_size)
{
	//jpeg_image* jpeg_image = malloc(sizeof(struct jpeg_image));
	jpeg_image jpeg_image;
	jpeg_decoder jpeg_decoder;
	for (int i = 0; i < 4; i++)
	{
		jpeg_decoder.huffman_slots[i] = 0;
		jpeg_decoder.quantization_table_slots[i] = 0;
	}

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
			CHECK_BUFFER_LENGTH(2, "Expected JPEG section size, but hit EOF!\n")

				jpeg_quantization_table_section* quantization_table_section = (jpeg_quantization_table_section*)&(image_data[current_buffer_position - 2]);
			uint64_t section_length = get_section_length(quantization_table_section);
			uint64_t current_section_length = current_buffer_position + 2;
			current_buffer_position += section_length;
			CHECK_BUFFER_BOUNDS("JPEG section exceeded file size!\n");

			while (current_section_length < current_buffer_position)
			{
				jpeg_quantization_table* quantization_table = &(image_data[current_section_length]);
				jpeg_decoder.quantization_table_slots[quantization_table->tq] = quantization_table;
				printf("Loading Quantization Table into Slot %i\n", quantization_table->tq);

				current_section_length += 65 + 64 * quantization_table->pq;
			}
			if (current_section_length != current_buffer_position)
				printf("WARNING: Quantization Tables not correctly loaded!\n");

			break;

		case JPEG_SEGMENT_PROGRESSIVE_DCT:;
			printf("Found Progressive DCT Section!\n");
			CHECK_BUFFER_LENGTH(2, "Expected JPEG section size, but hit EOF!\n")

				jpeg_frame_section* progressive_dct_section = (jpeg_frame_section*)&(image_data[current_buffer_position - 2]);
			current_buffer_position += get_section_length(progressive_dct_section);

			CHECK_BUFFER_BOUNDS("JPEG section exceeded file size!\n")

				return decode_progressive_jpeg_image(&jpeg_decoder, &jpeg_image, progressive_dct_section, current_buffer_position, image_data, buffer_size);
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