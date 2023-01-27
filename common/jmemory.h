#if !defined J_MEMORY_H
#define J_MEMORY_H

#include <windows.h>
#include <stdint.h>

void* Alloc(uint64_t Size)
{
    void *Result = VirtualAlloc(0, Size, MEM_RESERVE|MEM_COMMIT, PAGE_READWRITE); 
    
    return Result;
}

void Free(void *Memory)
{
    if(Memory)
    {
        VirtualFree(Memory, 0, MEM_RELEASE);
    }
    Memory = 0;
}

#endif