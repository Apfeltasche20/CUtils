#include "CNson.h"
#include <Windows.h>
#include <stdio.h>
#include <DynamicBuffer.h>

char* read_file(char* file)
{
	HANDLE file_handle = CreateFileA(file, GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	if (file_handle == 0)
	{
		printf("File %s could not be opened!\n", file);
		return 0;
	}

	LARGE_INTEGER file_size;
	BOOL success = GetFileSizeEx(file_handle, &file_size);
	if (!success)
	{
		printf("Error getting file size!\n");
		goto ERROR_CLOSE_HANDLE;
	}

	uint8_t* file_data = malloc(file_size.QuadPart + 1);
	if (file_data == 0)
	{
		printf("Error allocating memory for file reading\n");
		goto ERROR_CLOSE_HANDLE;
	}

	DWORD bytesRead = 0;
	success = ReadFile(file_handle, file_data, file_size.QuadPart, &bytesRead, 0);
	if (!success)
	{
		printf("Error reading file!\n");
		goto ERROR_FREE_BUFFER;
	}

	if (bytesRead != file_size.QuadPart)
	{
		printf("Not all bytes of file read!\n");
		goto ERROR_FREE_BUFFER;
	}
	file_data[file_size.QuadPart] = 0; // NULL TERMINATE DATA TO USE AS STRING
	return file_data;

ERROR_FREE_BUFFER:
	free(file_data);
ERROR_CLOSE_HANDLE:
	CloseHandle(file_handle);
	return 0;
}

void* checked_malloc(uint64_t size)
{
	void* memory = malloc(size);
	if (memory == 0)
	{
		printf("CNson: Error allocating 0x%llx Bytes of Memory!\n", size);
		exit(-1);
	}
	return memory;
}

char* nson_get_next_value(char* string, char** inner_string_start)
{
	dynamic_buffer* buffer = dynamic_buffer_create();

	uint64_t index = 0;
	uint8_t in_string = 0;
	uint8_t escaped = 0;
	while (string[index] != 0)
	{
		char current = string[index];
		if (escaped)
		{
			switch (current)
			{
			case 'n':
				dynamic_buffer_add(buffer, '\n');
				break;
			case 't':
				dynamic_buffer_add(buffer, '\t');
				break;

			default:
				dynamic_buffer_add(buffer, current);
				break;
			}
			
			escaped = 0;
		}
		else
		{
			switch (current)
			{
			case '\n':
				*inner_string_start = &(string[index + 1]);

				dynamic_buffer_add(buffer, 0);
				char* key = dynamic_buffer_copy_buffer(buffer);
				dynamic_buffer_free(buffer);
				return key;

			case '"':
				dynamic_buffer_add(buffer, current);
				in_string = !in_string;
				break;

			case '\\':
				escaped = 1;
				break;

			case ' ':
				if (in_string)
					dynamic_buffer_add(buffer, current);
				break;

			case '\r':
			case '\t':
				break;

			default:
				dynamic_buffer_add(buffer, current);
				break;
			}
		}

		index++;
	}

	dynamic_buffer_free(buffer);
	printf("Hit EOF while parsing Value for Key:\n\t%s\n", string);
	exit(-1);
}

char* nson_get_next_key(char* string, uint8_t* type, char** inner_string_start)
{
	dynamic_buffer* buffer = dynamic_buffer_create();

	uint64_t index = 0;
	while (string[index] != 0)
	{
		char current = string[index];
		switch (current)
		{
		case '{':
			*inner_string_start = &(string[index + 1]);
			*type = 1;

			dynamic_buffer_add(buffer, 0);
			char* object_key = dynamic_buffer_copy_buffer(buffer);
			dynamic_buffer_free(buffer);
			return object_key;

		case '=':
			*inner_string_start = &(string[index + 1]);
			*type = 2;

			dynamic_buffer_add(buffer, 0);
			char* key = dynamic_buffer_copy_buffer(buffer);
			dynamic_buffer_free(buffer);
			return key;
		
		case '}':
			*inner_string_start = &(string[index + 1]);
			*type = 3;
			dynamic_buffer_free(buffer);
			return 0;

		case '\n':
		case '\r':
		case '\t':
		case ' ':
			break;

		default:
			dynamic_buffer_add(buffer, current);
			break;
		}

		index++;
	}

	*type = 0;
	dynamic_buffer_free(buffer);
	return 0;
}

uint8_t nson_is_double(char* string, uint64_t string_len)
{
	uint64_t i = 0;
	if (string[i] == '-')
		i++;
	
	uint8_t dot = 0;
	for (i = 0; i < string_len; i++)
	{
		switch (string[i])
		{
		case '.':
			if (!dot)
				dot = 1;
			else
				return 0;

		case '0':
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		case '9':
			continue;

		default:
			return 0;
		}
	}

	return 1;
}

uint8_t nson_is_number(char* string, uint64_t string_len)
{
	uint64_t i = 0;
	if (string[i] == '-')
		i++;

	for (i = 0; i < string_len; i++)
	{
		switch (string[i])
		{
		case '0':
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		case '9':
			continue;

		default:
			return 0;
		}
	}

	return 1;
}

uint64_t nson_parse_value_string(char* value_string, nson_type* type)
{
	uint64_t value_string_len = strlen(value_string);
	
	if ((value_string[0] == '\"') && (value_string[value_string_len - 1] == '\"'))
	{
		// STRING
		*type = NSON_STRING;
		char* string_value = checked_malloc(value_string_len - 2 + 1);
		memcpy(string_value, &(value_string[1]), value_string_len - 1);
		string_value[value_string_len - 2] = 0;
		return (uint64_t) string_value;
	}
	if (!strncmp(value_string, "false", 5))
	{
		// BOOLEAN
		*type = NSON_BOOLEAN;
		return (uint64_t) 0;
	}
	if (!strncmp(value_string, "true", 4))
	{
		// BOOLEAN
		*type = NSON_BOOLEAN;
		return (uint64_t) 1;
	}
	if (nson_is_number(value_string, value_string_len))
	{
		// INTEGER
		*type = NSON_INT;
		return _atoi64(value_string);
	}
	if (nson_is_double(value_string, value_string_len))
	{
		// DOUBLE
		*type = NSON_DOUBLE;
		return atof(value_string);
	}
	

	printf("Can't get type of Value: \"%s\"\n", value_string);
	*type = NSON_UNKNOWN;
	return 0;
}

nson_object* nson_create_object(char* name)
{
	nson_object* object = checked_malloc(sizeof(struct nson_object));
	object->key = name;
	object->type = NSON_OBJECT;
	object->value = array_list_create();
	return object;
}

nson_string* nson_create_string(char* name, char* string)
{
	nson_string* object = checked_malloc(sizeof(struct nson_string));
	object->key = name;
	object->type = NSON_STRING;
	object->value = string;
	return object;
}

nson_boolean* nson_create_boolean(char* name, uint8_t boolean)
{
	nson_boolean* object = checked_malloc(sizeof(struct nson_boolean));
	object->key = name;
	object->type = NSON_BOOLEAN;
	object->value = boolean;
	return object;
}

nson_int* nson_create_int(char* name, uint64_t value)
{
	nson_int* object = checked_malloc(sizeof(struct nson_int));
	object->key = name;
	object->type = NSON_INT;
	object->value = value;
	return object;
}

nson_double* nson_create_double(char* name, double value)
{
	nson_double* object = checked_malloc(sizeof(struct nson_double));
	object->key = name;
	object->type = NSON_DOUBLE;
	object->value = value;
	return object;
}

char* nson_get_string(nson_object* object, char* name)
{
	uint64_t childs = array_list_size(object->value);
	for (uint32_t i = 0; i < childs; i++)
	{
		nson* nson = array_list_get(object->value, i);
		if (!strcmp(nson->key, name))
		{
			if (nson->type == NSON_STRING)
			{
				return ((nson_string*)nson)->value;
			}
			else
			{
				printf("Key \"%s\" in object \"%s\" was not of type string!\n", name, object->key);
				return "";
			}
		}
	}
	printf("Key \"%s\" not found in object \"%s\"!\n", name, object->key);
	return "";
}

nson_object* nson_get_object(nson_object* object, char* name)
{
	uint64_t childs = array_list_size(object->value);
	for (uint32_t i = 0; i < childs; i++)
	{
		nson* nson = array_list_get(object->value, i);
		if (!strcmp(nson->key, name))
		{
			if (nson->type == NSON_OBJECT)
			{
				return ((nson_object*)nson);
			}
			else
			{
				printf("Key \"%s\" in object \"%s\" was not of type object!\n", name, object->key);
				return 0;
			}
		}
	}
	printf("Key \"%s\" not found in object \"%s\"!\n", name, object->key);
	return 0;
}

uint8_t nson_get_boolean(nson_object* object, char* name)
{
	uint64_t childs = array_list_size(object->value);
	for (uint32_t i = 0; i < childs; i++)
	{
		nson* nson = array_list_get(object->value, i);
		if (!strcmp(nson->key, name))
		{
			if (nson->type == NSON_BOOLEAN)
			{
				return ((nson_boolean*)nson)->value;
			}
			else
			{
				printf("Key \"%s\" in object \"%s\" was not of type boolean!\n", name, object->key);
				return 0;
			}
		}
	}
	printf("Key \"%s\" not found in object \"%s\"!\n", name, object->key);
	return 0;
}

uint64_t nson_get_int(nson_object* object, char* name)
{
	uint64_t childs = array_list_size(object->value);
	for (uint32_t i = 0; i < childs; i++)
	{
		nson* nson = array_list_get(object->value, i);
		if (!strcmp(nson->key, name))
		{
			if (nson->type == NSON_INT)
			{
				return ((nson_int*)nson)->value;
			}
			else
			{
				printf("Key \"%s\" in object \"%s\" was not of type int!\n", name, object->key);
				return 0;
			}
		}
	}
	printf("Key \"%s\" not found in object \"%s\"!\n", name, object->key);
	return 0;
}

double nson_get_double(nson_object* object, char* name)
{
	uint64_t childs = array_list_size(object->value);
	for (uint32_t i = 0; i < childs; i++)
	{
		nson* nson = array_list_get(object->value, i);
		if (!strcmp(nson->key, name))
		{
			if (nson->type == NSON_DOUBLE)
			{
				return ((nson_double*)nson)->value;
			}
			else
			{
				printf("Key \"%s\" in object \"%s\" was not of type double!\n", name, object->key);
				return 0;
			}
		}
	}
	printf("Key \"%s\" not found in object \"%s\"!\n", name, object->key);
	return 0;
}

nson* nson_parse_key_value(char* key_name, char* string, char** output_rest_string)
{
	char* rest_string = string;

	char* value_string = nson_get_next_value(rest_string, &rest_string);
	//printf("Value of Key %s: %s\n", key_name, value_string);

	nson* return_nson = 0;
	uint64_t value_string_len = strlen(value_string);

	if ((value_string[0] == '\"') && (value_string[value_string_len - 1] == '\"'))
	{
		// STRING
		char* string_value = checked_malloc(value_string_len - 2 + 1);
		memcpy(string_value, &(value_string[1]), value_string_len - 1);
		string_value[value_string_len - 2] = 0;
		return_nson = nson_create_string(key_name, (char*)string_value);
	}
	else if (!strncmp(value_string, "false", 5))
	{
		// BOOLEAN
		return_nson = nson_create_boolean(key_name, 0);
	}
	else if (!strncmp(value_string, "true", 4))
	{
		// BOOLEAN
		return_nson = nson_create_boolean(key_name, 1);
	}
	else if (nson_is_number(value_string, value_string_len))
	{
		// INTEGER
		return_nson = nson_create_int(key_name, _atoi64(value_string));
	}
	else if (nson_is_double(value_string, value_string_len))
	{
		// DOUBLE
		return_nson = nson_create_double(key_name, atof(value_string));
	}
	else
	{
		printf("Ignoring Key \"%s\", because value \"%s\" could not be given an implemented Data Type!\n", key_name, value_string);
	}

	*output_rest_string = rest_string;
	free(value_string);
	
	return return_nson;
}

/*
* Parse a string into an existing Nson Object.
* 
* Args:
*	- nson_object*	object:				The Object to parse into
*	- char*			inner_string:		The String to parse into the object
*	- char**		output_rest_string:	The Rest of the String after the Nson Object was closed
*	- uint8_t		eof_is_terminate:	Interprete EOF as an End of the Nson Object. If 1 output_rest_string will not be set and can be 0
*/
void nson_parse_nson_object(nson_object* object, char* inner_string, char** output_rest_string, uint8_t eof_is_terminate)
{
	//printf("Parse Nson Object with Key \"%s\"", object->key);
	//printf("\n%s\n", inner_string);

	char* rest_string = inner_string;
	while(1)
	{ 
		uint8_t key_type = 0;
		char* next_key = nson_get_next_key(rest_string, &key_type, &rest_string);
		switch (key_type)
		{
		case 3: // CLOSE OBJECT
			*output_rest_string = rest_string;
			return;

		case 2:; // KEY-VALUE
			nson* key_value_pair = nson_parse_key_value(next_key, rest_string, &rest_string);
			if(key_value_pair != 0)
				array_list_add(object->value, key_value_pair);
			break;

		case 1:; // Object
			nson_object* new_nson_object = nson_create_object(next_key);
			nson_parse_nson_object(new_nson_object, rest_string, &rest_string, 0);
			array_list_add(object->value, new_nson_object);
			break;

		case 0:
			if (eof_is_terminate)
				return;

			printf("Hit EOF in nson_get_next_key!\n");
			exit(-1);
		}
	}
}

nson_object* nson_parse_string(char* string)
{

}

nson_object* nson_parse_string_and_pack(char* string)
{
	nson_object* outer_file = nson_create_object("file");
	char* output = 0;
	nson_parse_nson_object(outer_file, string, 0, 1);
	return outer_file;
}

nson_object* nson_read_file(char* file)
{
	char* string = read_file(file);
	
	nson* value = nson_parse_string(string);
	
	free(string);
	return value;
}

nson_object* nson_read_file_and_pack(char* file)
{
	char* string = read_file(file);

	nson* value = nson_parse_string_and_pack(string);

	free(string);
	return value;
}

int main()
{
	nson_object* file_object = nson_read_file_and_pack("test.nson");

	printf("%s\n", nson_get_string(nson_get_object(file_object, "player_state"), "current_map"));
	
	return 0;
}
