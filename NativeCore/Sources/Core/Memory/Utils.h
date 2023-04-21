//
// Created by stuka on 13.04.2023.
//

#ifndef NATIVECORE_UTILS_H
#define NATIVECORE_UTILS_H

#pragma once
#include <windows.h>
#include <psapi.h>

namespace Utils
{
    void get_byte_process_mem_usage(size_t &pagefile_usage, size_t &working_set_size)
    {
        PROCESS_MEMORY_COUNTERS memCounter;
        BOOL result = GetProcessMemoryInfo(GetCurrentProcess(),
                                           &memCounter,
                                           sizeof( memCounter ));

        pagefile_usage = memCounter.PagefileUsage;
        working_set_size = memCounter.WorkingSetSize;
    }
    void get_kilobyte_process_mem_usage(size_t &pagefile_usage, size_t &working_set_size)
    {
        get_byte_process_mem_usage(pagefile_usage, working_set_size);
        pagefile_usage /= 1024;
        working_set_size /= 1024;
    }
    void get_megabyte_process_mem_usage(size_t &pagefile_usage, size_t &working_set_size)
    {
        get_byte_process_mem_usage(pagefile_usage, working_set_size);
        pagefile_usage /= 1024 * 1024;
        working_set_size /= 1024 * 1024;
    }
}


#endif //NATIVECORE_UTILS_H
