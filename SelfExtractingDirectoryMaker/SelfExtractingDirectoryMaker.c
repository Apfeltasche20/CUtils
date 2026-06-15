#include <Windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <shlwapi.h>
#include <stdint.h>
#include <FileSystemPacker.h>

typedef struct file_information {
    char signature[4];
    uint32_t file_offset;
    uint32_t start_file_name_length;
    uint32_t payload_size;
} file_information;

uint8_t* read_file_handle(HANDLE file_handle, uint64_t* size)
{
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

    *size = file_size.QuadPart;
    return image_data;

ERROR_CLOSE_HANDLE:
    return 0;
}

int main(int argc, char** argv)
{
    /*
    HANDLE own_handle = GetModuleHandleA(0);

    file_information* header = (file_information*)(((uint64_t)own_handle) + 4);
    if (strncmp(header->signature, "PACK", 4))
    {
        printf("No Attached Archive found!\n");
        exit(-1);
    }

    char* start_file_name = ((uint64_t)own_handle) + header->file_offset;
    uint8_t* file_data = ((uint64_t)own_handle) + header->file_offset + header->start_file_name_length;
    if (!PathFileExistsA(start_file_name))
    {

    }
    */

    HANDLE own_handle = GetModuleHandleA(0);
    file_information* own_pack_header = (file_information*)(((uint64_t)own_handle) + 4);
    if (strncmp(own_pack_header->signature, "PACK", 4))
    {
        printf("No Attached Archive found!\n");
        exit(-1);
    }

    HANDLE ownFile = CreateFileA(argv[0], GENERIC_READ, FILE_SHARE_READ, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
    SetFilePointer(ownFile, own_pack_header->file_offset + own_pack_header->start_file_name_length, 0, FILE_BEGIN);

    uint8_t* file_data = malloc(own_pack_header->payload_size);
    DWORD read;
    BOOL success = ReadFile(ownFile, file_data, own_pack_header->payload_size, &read, 0);
    printf("Read Extracing Exe: %i\n", success);

    CloseHandle(ownFile);

    
    //uint64_t exe_size;
    //uint8_t* exe = read_file(argv[1], &exe_size);
    uint64_t exe_size = own_pack_header->payload_size;
    uint8_t* exe = file_data;

    //HANDLE tempArchive = CreateFileA("archive.pack", GENERIC_READ, FILE_SHARE_WRITE, 0, OPEN_ALWAYS, FILE_ATTRIBUTE_TEMPORARY, 0);
    pack_directory(argv[1], "archive.pack");
    
    HANDLE tempArchive = CreateFileA("archive.pack", GENERIC_READ, 0, 0, OPEN_ALWAYS, FILE_ATTRIBUTE_TEMPORARY, 0);

    uint64_t dircetory_size;
    uint8_t* dircetory = read_file_handle(tempArchive, &dircetory_size);

    CloseHandle(tempArchive);
    
    printf("Read Directory: 0x%llx\n", dircetory);

    char* start_file_name = argv[2];
    uint32_t start_file_name_length = 0;
    if (start_file_name != 0)
        start_file_name_length = strlen(start_file_name) + 1;

    file_information* header = (file_information*)((uint64_t)exe + 4);
    header->file_offset = exe_size;
    header->signature[0] = 'P';
    header->signature[1] = 'A';
    header->signature[2] = 'C';
    header->signature[3] = 'K';
    header->start_file_name_length = start_file_name_length;
    header->payload_size = dircetory_size;

    HANDLE output_file = CreateFileA("output.exe", GENERIC_WRITE, 0, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
    DWORD written;
    WriteFile(output_file, exe, exe_size, &written, 0);
    if(start_file_name_length > 0)
        WriteFile(output_file, start_file_name, start_file_name_length, &written, 0);
    WriteFile(output_file, dircetory, dircetory_size, &written, 0);
    CloseHandle(output_file);

    free(file_data);
    return 0;
}