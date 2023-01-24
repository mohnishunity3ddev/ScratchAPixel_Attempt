#if !defined(__FILE_READER_H_)
#define __FILE_READER_H_

#include <windows.h>
#include <stdint.h>
#include <fstream>
#include <iostream>
#include <cstdio>

struct read_result
{
    uint32_t ContentSize;
    void *Contents;
};

// NOTE: The Win32 Version
static read_result
JReadFileWin32(const char *Filename)
{
    read_result Result = {};
    HANDLE FileHandle = CreateFileA(Filename, GENERIC_READ, FILE_SHARE_READ, 0,
                                    OPEN_EXISTING, 0, 0);
    
    if(FileHandle != INVALID_HANDLE_VALUE)
    {
        LARGE_INTEGER FileSize;
        if(GetFileSizeEx(FileHandle, &FileSize))
        {
            uint32_t FileSize32 = (uint32_t)FileSize.QuadPart;
            Result.Contents = VirtualAlloc(0, FileSize32, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);

            if(Result.Contents)
            {
                DWORD BytesRead;
                if(ReadFile(FileHandle, Result.Contents, FileSize32, &BytesRead, 0) && BytesRead == FileSize32)
                {
                    Result.ContentSize = BytesRead;
                }
                else
                {
                    VirtualFree(Result.Contents, 0, MEM_RELEASE);
                }
            }

            CloseHandle(FileHandle);
        }
    }                               

    return Result;     
}

// NOTE: The C++ Version
static read_result
JReadFile(const char *Filename)
{
    read_result Result = {};
    std::ifstream file(Filename, std::ios::binary | std::ios::ate);
    if(!file.is_open())
    {
        std::cout << "Error opening file: " << Filename << std::endl;
        return Result;
    }
    
    // get the size of the file.
    std::streampos size = file.tellg();
    
    // get the file pointer back to the beginning of the file.
    file.seekg(0, std::ios::beg);

    if(size > 0)
    {
        Result.Contents = malloc(size);
        
        if(Result.Contents)
        {
            file.read((char *)Result.Contents, size);
            
            uint32_t BytesRead = file.gcount();
            if(BytesRead == size)
            {
                Result.ContentSize = (uint32_t) size;
            }
            else
            {
                free(Result.Contents);
                Result.ContentSize = 0;
                Result.Contents = 0;
            }
        }
    
    }

    file.close();
    return Result;
}

// NOTE: The C Version
static read_result
JReadFileC(const char *Filename)
{
    read_result Result = {};

    FILE *fp = fopen(Filename, "rb");
    if(fp == 0)
    {
        printf("There was an error opening file: %s\n", Filename);
        return Result;
    }

    fseek(fp, 0, SEEK_END);
    size_t size = ftell(fp);
    rewind(fp);

    if(size > 0)
    {
        Result.Contents = malloc(size);
        
        if(Result.Contents)
        {
            size_t BytesRead =
                fread((uint8_t *)Result.Contents, sizeof(uint8_t), size, fp);
            if(BytesRead == size)
            {
                Result.ContentSize = (uint32_t) size;
            }
            else
            {
                free(Result.Contents);
                Result.ContentSize = 0;
                Result.Contents = 0;
            }
        }
    }

    fclose(fp);
    return Result;
}

static void
WriteTestPPMFile(uint32_t width, uint32_t height)
{
    std::ofstream ofs("test_ppm.ppm", std::ios::out | std::ios::binary);
    unsigned char max = 0xff;

    ofs << "P6\n" << width << " " << height << "\n255\n";
    for (unsigned i = 0; i < height; ++i)
    {
        for (unsigned j = 0; j < width; ++j)
        {
            if (i < height / 2)
            {
                if (j < width / 2)
                {
                    ofs << (unsigned char)255 << (unsigned char)0
                        << (unsigned char)0;
                }
                else
                {
                    ofs << (unsigned char)0 << (unsigned char)255
                        << (unsigned char)0;
                }
            }
            else
            {
                if (j < width / 2)
                {
                    ofs << (unsigned char)0 << (unsigned char)0
                        << (unsigned char)255;
                }
                else
                {
                    ofs << (unsigned char)255 << (unsigned char)255
                        << (unsigned char)255;
                }
            }
        }
    }
    ofs.close();
}

#endif