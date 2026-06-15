#include "filesystem.h"
#include <stdio.h>
#include <string.h>
#include <wchar.h>
#include <stdlib.h>
#include <io.h>
#include "util.h"

wchar_t* root_replace = L"D:\\Code\\RenPyEmulator\\RenPyEmulator\\virtualroot";
//wchar_t *default_directory = L"C:\\EroDungeonsWindows\\";
wchar_t* default_directory = L"C:\\SuccubusTakeover-2.0-pc\\";
wchar_t** current_directory = &default_directory;

void replace_slashes_with_backslashes(wchar_t* internal_path)
{
    for (int i = 0;; i++)
    {
        if (internal_path[i] == 0)
            return;

        if (internal_path[i] == '/')
            internal_path[i] = '\\';
    }
}

wchar_t* set_virtual_root(wchar_t* external_path)
{
    if ((external_path[0] == '\\') && (external_path[1] == '\\') && (external_path[2] == '?') && (external_path[3] == '\\'))
    {
        int root_length = wcslen(root_replace) - 12;
        int new_path_length = wcslen(external_path) - root_length;

        wchar_t* new_buffer = malloc((new_path_length + 1 + 2) * 2);
        memcpy(new_buffer + (6), external_path + root_length + 4, (new_path_length + 1 - 4) * 2);
        new_buffer[0] = '\\';
        new_buffer[1] = '\\';
        new_buffer[2] = '?';
        new_buffer[3] = '\\';
        new_buffer[4] = 'C';
        new_buffer[5] = ':';

        return new_buffer;
    }
    else
    {
        int root_length = wcslen(root_replace) - 12;
        int new_path_length = wcslen(external_path) - root_length;

        wchar_t* new_buffer = malloc((new_path_length + 1 + 2) * 2);
        memcpy(new_buffer + 2, external_path + root_length, (new_path_length + 1) * 2);
        new_buffer[0] = 'C';
        new_buffer[1] = ':';

        return new_buffer;
    }
}

unsigned char* replace_virtual_root(wchar_t* internal_path)
{
    uint64_t length = wcslen(internal_path);

    uint64_t root_length = wcslen(root_replace);
    uint64_t new_length = length + root_length - 2;

    unsigned char* real_path = malloc((new_length + 1));
    for (int i = 0; i < (root_length + 1); i++)
    {
        real_path[i] = root_replace[i];
    }
    int offset = 0;
    for (int i = 0; i < (length - 2); i++)
    {
        if ((unsigned char)internal_path[i + 2] == '.')
        {
            if ((unsigned char)internal_path[i + 3] == '\\')
            {
                if ((unsigned char)internal_path[i + 1] == '\\')
                {
                    i += 2;
                    offset += 2;
                }
            }
        }
        
        real_path[i + root_length - offset] = (unsigned char)internal_path[i + 2];
        
    }
    real_path[new_length - offset] = 0;
    //memcpy(real_path, root_replace, 31 * 2);
    //memcpy(real_path + 31, internal_path + 2, (length - 2 + 1) * 2);
    printf("%s\n", real_path);
    return real_path;
}

wchar_t* add_current_folder(wchar_t* internal_path)
{
    uint64_t length = wcslen(internal_path);
    uint64_t current_path_length = wcslen(*current_directory);
    uint64_t need_slash = 1;
    if (((*current_directory)[current_path_length - 1] == '\\') || ((*current_directory)[current_path_length - 1] == '/'))
        need_slash = 0;

    uint64_t new_length = length + current_path_length + need_slash;

    wchar_t* real_path = malloc((new_length + 1) * 2);
    for (int i = 0; i < current_path_length; i++)
    {
        real_path[i] = (*current_directory)[i];
    }
    if (need_slash)
        real_path[current_path_length] = '\\';
    for (int i = 0; i < length; i++)
    {
        real_path[i + current_path_length + need_slash] = internal_path[i];
    }
    real_path[new_length] = 0;

    return real_path;
}

wchar_t* make_internal_absolut_w(wchar_t* internal_path)
{
    if (internal_path[0] == '~')
    {
        uint64_t length = wcslen(internal_path);
        wchar_t* internal_path_copy = malloc((length + 2) * 2);
        memcpy(internal_path_copy + 1, internal_path, (length + 2) * 2);
        internal_path_copy[0] = 'C';
        internal_path_copy[1] = ':';
        return internal_path_copy;
    }
    
    if(internal_path[0] == '\\')
        internal_path = internal_path + 1;
    
    if ((internal_path[0] == '.') && (internal_path[1] == '\\'))
    {
        internal_path = internal_path + 2;
    }

    if (((internal_path[0] != 'C') && (internal_path[0] != 'c')) || (internal_path[1] != ':')) // relativ path
    {
        uint64_t length = wcslen(internal_path);
        wchar_t* internal_path_copy = malloc((length + 1) * 2);
        memcpy(internal_path_copy, internal_path, (length + 1) * 2);

        replace_slashes_with_backslashes(internal_path_copy);

        internal_path = add_current_folder(internal_path_copy);
        return internal_path;
    }

    uint64_t length = wcslen(internal_path);
    wchar_t* internal_path_copy = malloc((length + 1) * 2);
    memcpy(internal_path_copy, internal_path, (length + 1) * 2);
    return internal_path_copy;
    //return internal_path;
}

wchar_t* make_internal_absolut(char* internal_path)
{
    wchar_t* temp = char_to_wchar(internal_path);
    wchar_t* result = make_internal_absolut_w(temp);
    free(temp);
    return result;
}

unsigned char* get_file_from_internal_file_path(char* internal_path)
{
    wchar_t* temp = char_to_wchar(internal_path);
    char* result = get_file_from_internal_file_path_w(temp);
    free(temp);
    return result;
}

unsigned char* get_file_from_internal_file_path_w(wchar_t* internal_path)
{
    if ((internal_path[0] == '\\') && (internal_path[1] == '\\') && (internal_path[2] == '?') && (internal_path[3] == '\\'))
    {
        internal_path += 4;
    }
    
    if (internal_path[0] == '~')
    {
        uint64_t length = wcslen(internal_path);
        wchar_t* temp_copy = malloc((length + 2) * 2);
        memcpy(temp_copy + 1, internal_path, (length + 2) * 2);
        temp_copy[0] = 'C';
        temp_copy[1] = ':';
        internal_path = temp_copy;
    }

    if (internal_path[0] == '\\')
        internal_path = internal_path + 1;
    
    if ((internal_path[0] == '.') && (internal_path[1] == '\\'))
    {
        internal_path = internal_path + 2;
    }

    if (((internal_path[0] != 'C') && (internal_path[0] != 'c')) || (internal_path[1] != ':')) // relativ path
    {
        uint64_t length = wcslen(internal_path);
        wchar_t* internal_path_copy = malloc((length + 1) * 2);
        memcpy(internal_path_copy, internal_path, (length + 1) * 2);

        replace_slashes_with_backslashes(internal_path_copy);

        internal_path = add_current_folder(internal_path_copy);
    }

    uint64_t length = wcslen(internal_path);
    wchar_t* internal_path_copy = malloc((length + 1) * 2);
    memcpy(internal_path_copy, internal_path, (length + 1) * 2);

    replace_slashes_with_backslashes(internal_path_copy);

    unsigned char* result_path = replace_virtual_root(internal_path_copy);
    free(internal_path_copy);
    return result_path;
}

wchar_t* get_current_directory()
{
    return *current_directory;
}

uint64_t set_current_dirctory(wchar_t* path)
{
    if (!wcscmp(path, L"."))
    {
        return 1;
    }

    if ((path[0] != 'C') || (path[1] != ':')) // relativ path
    {
        uint64_t length = wcslen(path);
        wchar_t* internal_path_copy = malloc((length + 1) * 2);
        memcpy(internal_path_copy, path, (length + 1) * 2);

        replace_slashes_with_backslashes(internal_path_copy);

        path = add_current_folder(internal_path_copy);
    }

    if (current_directory != &default_directory)
    {
        free(*current_directory);
    }

    uint64_t length = wcslen(path);

    uint64_t add_back_shlash = 1;
    if ((path[length - 1] == '\\') || (path[length - 1] == '/'))
        add_back_shlash = 0;

    wchar_t* copy = malloc((length + 1 + add_back_shlash) * 2);
    memcpy(copy, path, (length + 1) * 2);

    if (add_back_shlash)
    {
        copy[length] = '\\';
        copy[length + 1] = 0;
    }

    replace_slashes_with_backslashes(copy);

    // check if path exists

    char* real_path = get_file_from_internal_file_path_w(copy);
    uint64_t attrib = GetFileAttributesA(real_path);
    uint64_t exists = ((attrib != -1) && (attrib & 0x10));
    if (exists)
    {
        *current_directory = copy;
        return 1;
    }
    else
    {
        return 0;
    }
}

wchar_t* find_python_dll(wchar_t* path_template, wchar_t* dll_dir)
{
    int minor_version = 0;

    for (int major_version = 1; major_version <= 9; major_version++)
    {
        for (int minor_version = 0; minor_version <= 20; minor_version++)
        {
            wchar_t* path_buffer = malloc(0x255 * 2);
            swprintf_s(path_buffer, 0x255, path_template, dll_dir, major_version, minor_version);

            char* real_path = get_file_from_internal_file_path_w(path_buffer);
            if (_access(real_path, 0) == 0)
            {
                free(real_path);
                return path_buffer;
            }
            else
            {
                free(real_path);
                free(path_buffer);
            }
        }
    }
}