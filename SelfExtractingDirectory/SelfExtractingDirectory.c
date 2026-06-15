#include "SelfExtractingDirectory.h"
#include <Windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <shlwapi.h>
#include <FileSystemPacker.h>

int main(int argc, char** argv)
{
    HANDLE own_handle = GetModuleHandleA(0);

    file_information* header = (file_information*)(((uint64_t)own_handle) + 4);
    if (strncmp(header->signature, "PACK", 4))
    {
        printf("No Attached Archive found!\n");
        exit(-1);
    }

    uint8_t has_start_file = (header->start_file_name_length != 0);
    char* start_file_name = 0;

    if (has_start_file)
    {
        HANDLE ownFile = CreateFileA(argv[0], GENERIC_READ, FILE_SHARE_READ, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
        SetFilePointer(ownFile, header->file_offset, 0, FILE_BEGIN);

        start_file_name = malloc(header->start_file_name_length);
        DWORD read;
        ReadFile(ownFile, start_file_name, header->start_file_name_length, &read, 0);

        CloseHandle(ownFile);
    }

    if ((!has_start_file) || ((start_file_name != 0) && (!PathFileExistsA(start_file_name))))
    {
        HANDLE ownFile = CreateFileA(argv[0], GENERIC_READ, FILE_SHARE_READ, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
        SetFilePointer(ownFile, header->file_offset + header->start_file_name_length, 0, FILE_BEGIN);

        uint8_t* file_data = malloc(header->payload_size);
        DWORD read;
        ReadFile(ownFile, file_data, header->payload_size, &read, 0);

        //uint8_t* file_data = (uint8_t*)(((uint64_t)own_handle) + header->file_offset + header->start_file_name_length);
        unpack_directory_data(".", file_data, 0);

        free(file_data);
        CloseHandle(ownFile);
    }

    if (has_start_file)
    {
        Sleep(1);
        printf("Starting Process %s\n", start_file_name);
        
        PROCESS_INFORMATION info;
        STARTUPINFOA startupinfo;
        GetStartupInfoA(&startupinfo);
        BOOL result = CreateProcessA(start_file_name, 0, NULL, NULL, FALSE, /* CREATE_SECURE_PROCESS | */ CREATE_SUSPENDED, NULL, NULL, &startupinfo, &info);
        printf("result: %i", GetLastError());

        ResumeThread(info.hThread);
        WaitForSingleObject(info.hThread, INFINITE);

        CloseHandle(info.hThread);
        CloseHandle(info.hProcess);

        free(start_file_name);
    }
}
