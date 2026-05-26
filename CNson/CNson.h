#pragma once
#include <stdint.h>
#include <List.h>

typedef enum nson_type {
	NSON_UNKNOWN, NSON_OBJECT, NSON_INT, NSON_BOOLEAN, NSON_BYTE, NSON_CHAR, NSON_DOUBLE, NSON_STRING
} nson_type;

typedef struct nson {
	nson_type type;
	char* key;

	uint64_t value;
} nson;

typedef struct nson_int {
	nson_type type;
	char* key;

	uint64_t value;
} nson_int;

typedef struct nson_boolean {
	nson_type type;
	char* key;

	uint8_t value;
} nson_boolean;

typedef struct nson_byte {
	nson_type type;
	char* key;

	uint8_t value;
} nson_byte;

typedef struct nson_char {
	nson_type type;
	char* key;

	uint8_t value;
} nson_char;

typedef struct nson_double {
	nson_type type;
	char* key;

	double value;
} nson_double;

typedef struct nson_string {
	nson_type type;
	char* key;

	char* value;
} nson_string;

typedef struct nson_object {
	nson_type type;
	char* key;

	array_list* value;
} nson_object;

nson_object* nson_create_object(char* name);
nson_string* nson_create_string(char* name, char* string);
nson_boolean* nson_create_boolean(char* name, uint8_t boolean);
nson_int* nson_create_int(char* name, uint64_t value);
nson_double* nson_create_double(char* name, double value);

char* nson_get_string(nson_object* object, char* name);
nson_object* nson_get_object(nson_object* object, char* name);
uint8_t nson_get_boolean(nson_object* object, char* name);
uint64_t nson_get_int(nson_object* object, char* name);
double nson_get_double(nson_object* object, char* name);

nson_object* nson_parse_string(char* string);
nson_object* nson_parse_string_and_pack(char* string);
nson_object* nson_read_file(char* file);
nson_object* nson_read_file_and_pack(char* file);