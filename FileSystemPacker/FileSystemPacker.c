#include "FileSystemPacker.h"
#include <shlwapi.h>
#include <Windows.h>
#include <stdio.h>
#include <HuffmanCompressor.h>

uint8_t* packed_filesystem_read_file(char* file_name, uint64_t* size)
{
    HANDLE file_handle = CreateFileA(file_name, GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
    if (file_handle == -1)
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
    }

    CloseHandle(file_handle);
    *size = file_size.QuadPart;
    return image_data;

ERROR_CLOSE_HANDLE:
    CloseHandle(file_handle);
    return 0;
}

void pack_handle_file(char* file, char* name, HANDLE output_file)
{
    uint64_t file_size;
    uint8_t* data = packed_filesystem_read_file(file, &file_size);
    uint64_t string_size = strlen(name) + 1;
    
    packed_filesystem_entry_file entry_header;

    huffman_tree_memory_allocation_struct* huffman_tree_memory;
    huffman_tree_node* huffman_tree;
    uint64_t output_size;
    uint8_t filling_bits;
    uint8_t* encoded_data = huffman_encode_data(data, file_size, &huffman_tree, &output_size, &huffman_tree_memory, &filling_bits);

    uint32_t encoded_size = output_size + huffman_tree_memory->current_size;

    if (encoded_size < file_size)
    {
        entry_header.type = PACKED_FILESYSTEM_ENTRY_FILE;
        entry_header.flags = PACKED_FILESYSTEM_FLAG_FILE_HUFFMAN;
        entry_header.compressed_file_size = output_size;
        entry_header.uncompressed_file_size = file_size;
        entry_header.data_size = output_size + huffman_tree_memory->current_size;
        entry_header.offset_to_data = string_size;

        uint64_t temp = ((uint64_t)huffman_tree_memory->memory + huffman_tree_memory->current_max_size) - (uint64_t)huffman_tree;
        printf("%s, %i, %i\n", name, huffman_tree->one_offset, huffman_tree->zero_offset);

        uint32_t written;
        WriteFile(output_file, &entry_header, sizeof(packed_filesystem_entry_file), &written, 0);
        WriteFile(output_file, name, string_size, &written, 0);
        WriteFile(output_file, encoded_data, output_size, &written, 0);
        WriteFile(output_file, huffman_tree, huffman_tree_memory->current_size, &written, 0);
    }
    else
    {
        entry_header.type = PACKED_FILESYSTEM_ENTRY_FILE;
        entry_header.flags = PACKED_FILESYSTEM_FLAG_FILE_NORMAL;
        entry_header.compressed_file_size = file_size;
        entry_header.uncompressed_file_size = file_size;
        entry_header.data_size = file_size;
        entry_header.offset_to_data = string_size;

        uint32_t written;
        WriteFile(output_file, &entry_header, sizeof(packed_filesystem_entry_file), &written, 0);
        WriteFile(output_file, name, string_size, &written, 0);
        WriteFile(output_file, data, file_size, &written, 0);
    }

    free(encoded_data);
    free(huffman_tree_memory->memory);
}

void pack_handle_directory(char* directory, char* name, HANDLE output_file)
{
    WIN32_FIND_DATAA output;
    char search_directory[1024];
    char new_file_name[1024];
    sprintf_s(search_directory, 1024, "%s\\*", directory);

    uint64_t elements = 0;
    HANDLE current_entry = FindFirstFileA(search_directory, &output);
    while (current_entry != INVALID_HANDLE_VALUE)
    {
        if (!strcmp(".", output.cFileName))
        {
            goto count_loop;
        }
        if (!strcmp("..", output.cFileName))
        {
            goto count_loop;
        }

        elements++;

    count_loop:;
        BOOL success = FindNextFileA(current_entry, &output);
        if (!success)
            break;
    }

    uint64_t string_size = strlen(name) + 1;

    packed_filesystem_entry_directory entry_header;
    entry_header.type = PACKED_FILESYSTEM_ENTRY_DIRECTORY;
    entry_header.flags = PACKED_FILESYSTEM_FLAG_DIRECTORY_NORMAL;
    entry_header.child_entries = elements;
    entry_header.unused = 0;
    entry_header.offset_to_data = string_size;

    uint32_t written;
    WriteFile(output_file, &entry_header, sizeof(packed_filesystem_entry_directory), &written, 0);
    WriteFile(output_file, name, string_size, &written, 0);

    current_entry = FindFirstFileA(search_directory, &output);

    while (current_entry != INVALID_HANDLE_VALUE)
    {
        if (!strcmp(".", output.cFileName))
        {
            goto next_loop;
        }
        if (!strcmp("..", output.cFileName))
        {
            goto next_loop;
        }

        sprintf_s(new_file_name, 1024, "%s\\%s", directory, output.cFileName);
        BOOL is_directory = PathIsDirectoryA(new_file_name); 
        //HANDLE new_file_handle = CreateFileA(new_file_name, )

        //printf("%s: %i\n", new_file_name, is_directory);
        if (is_directory)
        {
            pack_handle_directory(new_file_name, output.cFileName, output_file);
        }
        else
        {
            pack_handle_file(new_file_name, output.cFileName, output_file);
        }

    next_loop:;
        BOOL success = FindNextFileA(current_entry, &output);
        if (!success)
            break;
    }
}

void pack_directory(char* directory, char* output_file)
{
    HANDLE outputFile = CreateFileA(output_file, GENERIC_WRITE, 0, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);

    packed_filesystem_file_header header;
    header.signature[0] = 'P';
    header.signature[1] = 'A';
    header.signature[2] = 'C';
    header.signature[3] = 'K';
    header.unused = 0;
    header.version = 1;
    header.offset_to_root_dir = 0;

    uint32_t written;
    WriteFile(outputFile, &header, sizeof(packed_filesystem_file_header), &written, 0);

    pack_handle_directory(directory, "root", outputFile);

    CloseHandle(outputFile);
    return;
}

uint8_t* unpack_file_entry(char* output_directory, uint8_t* data, packed_filesystem_entry_file* file)
{
    if (file->type != PACKED_FILESYSTEM_ENTRY_FILE)
    {
        printf("File is not a file!\n");
        return;
    }

    char dir[1024];
    sprintf_s(dir, 1024, "%s\\%s", output_directory, file->name);

    HANDLE handle = CreateFileA(dir, GENERIC_WRITE, 0, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
    uint8_t* file_data = ((uint64_t)file) + file->offset_to_data + sizeof(struct packed_filesystem_entry_file);
    if (file->flags & PACKED_FILESYSTEM_FLAG_FILE_NORMAL)
    {
        DWORD written = 0;
        WriteFile(handle, file_data, file->compressed_file_size, &written, 0);
    }
    else if (file->flags & PACKED_FILESYSTEM_FLAG_FILE_HUFFMAN)
    {
        huffman_tree_node* root = (huffman_tree_node*)((uint64_t)file_data + file->compressed_file_size);

        printf("%s, %i, %i\n", dir, root->one_offset, root->zero_offset);

        uint8_t* uncompressed = huffman_decode_data(file_data, file->compressed_file_size, root, file->uncompressed_file_size);

        DWORD written = 0;
        WriteFile(handle, uncompressed, file->uncompressed_file_size, &written, 0);

        free(uncompressed);
    }
    CloseHandle(handle);

    return ((uint64_t)file) + file->offset_to_data + sizeof(struct packed_filesystem_entry_file) + file->data_size;
}

uint8_t* unpack_directory_entry(char* output_directory, uint8_t* data, packed_filesystem_entry_directory* directory)
{
    if (directory->type != PACKED_FILESYSTEM_ENTRY_DIRECTORY)
    {
        printf("Root Directory is not a directory!\n");
        return;
    }
    
    char dir[1024];
    sprintf_s(dir, 1024, "%s\\%s", output_directory, directory->name);

    BOOL success = CreateDirectoryA(dir, 0);
    if (!success)
    {
        printf("Error creating Directory %s!\n", dir);
    }

    uint8_t* current_data = ((uint64_t)directory) + directory->offset_to_data + sizeof(struct packed_filesystem_entry_directory);
    for (uint32_t i = 0; i < directory->child_entries; i++)
    {
        packed_filesystem_entry* entry = (packed_filesystem_entry*)current_data;
        switch (entry->type)
        {
        case PACKED_FILESYSTEM_ENTRY_DIRECTORY:
            current_data = unpack_directory_entry(dir, data, entry);
            break;
        case PACKED_FILESYSTEM_ENTRY_FILE:
            current_data = unpack_file_entry(dir, data, entry);
            break;
        default:
            printf("Unknown Filesystem entry!\n");
            break;
        }
    }

    return current_data;
}

void unpack_root_directory_entry(char* output_directory, uint8_t* data, packed_filesystem_entry_directory* directory)
{
    if (directory->type != PACKED_FILESYSTEM_ENTRY_DIRECTORY)
    {
        printf("Root Directory is not a directory!\n");
        return;
    }

    uint8_t* current_data = ((uint64_t)directory) + directory->offset_to_data + sizeof(struct packed_filesystem_entry_directory);
    for (uint32_t i = 0; i < directory->child_entries; i++)
    {
        packed_filesystem_entry* entry = (packed_filesystem_entry*)current_data;
        switch (entry->type)
        {
        case PACKED_FILESYSTEM_ENTRY_DIRECTORY:
            current_data = unpack_directory_entry(output_directory, data, entry);
            break;
        case PACKED_FILESYSTEM_ENTRY_FILE:
            current_data = unpack_file_entry(output_directory, data, entry);
            break;
        default:
            printf("Unknown Filesystem entry!\n");
            break;
        }
    }
}

void unpack_directory_data(char* output_directory, uint8_t* data, uint64_t data_length)
{
    packed_filesystem_file_header* file_header = (packed_filesystem_file_header*)data;
    if (strncmp(file_header->signature, "PACK", 4))
    {
        printf("File Signature not found!\n");
        return;
    }

    if (file_header->version != 1)
    {
        printf("Unknown File Version found!\n");
    }

    unpack_root_directory_entry(output_directory, data, &data[sizeof(packed_filesystem_file_header) + file_header->offset_to_root_dir]);
}

void unpack_directory(char* output_directory, char* file)
{
    uint64_t file_size;
    uint8_t* data = packed_filesystem_read_file(file, &file_size);

    unpack_directory_data(output_directory, data, file_size);

    free(data);
}

/*
int main()
{
    //pack_directory("D:\\Code\\CUtils\\HuffmanCompressor", "output-compressed.bin");
    //unpack_directory("output", "output-compressed.bin");
}
*/
