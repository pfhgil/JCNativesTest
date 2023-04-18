//
// Created by stuka on 17.04.2023.
//

#ifndef TEST00_MEMORYMANAGER_H
#define TEST00_MEMORYMANAGER_H

#include "Allocators/FrameAllocator.h"
#include <windows.h>
#include <unistd.h>

// defines

// ----------

using Memory::Allocators::FrameAllocator;

namespace Memory::MemoryManager
{
    std::uint64_t get_device_free_mem();

    class Settings
    {
    private:
        // максимальный коэффициент памяти (от свободной памяти), которое может использовать движок
        static inline float max_free_mem_usage_coeff = 0.5f;


        // кол-во свободной памяти на старте
        static inline std::uint64_t start_free_mem = get_device_free_mem();

        // максимальное кол-во используемой памяти
        static inline std::uint64_t max_mem_usage = start_free_mem * max_free_mem_usage_coeff;

        // allocators settings ---------------------
        // максимальный коэффициент памяти, который может использовать frame allocator (от максимальной занимаемой движком памяти)
        static inline float max_frame_allocator_mem_usage_coeff = 0.01f;
        static inline std::uint64_t max_frame_allocator_mem_usage = max_mem_usage * max_frame_allocator_mem_usage_coeff;

    public:
        static inline std::uint64_t get_start_free_mem() noexcept { return start_free_mem; }
        static inline std::uint64_t get_max_mem_usage() noexcept { return max_mem_usage; }
        static inline std::uint64_t get_max_frame_allocator_mem_usage() noexcept { return max_frame_allocator_mem_usage; }

        static void print_info() noexcept
        {
            std::uint64_t start = get_start_free_mem();
            std::uint64_t max_mem = get_max_mem_usage();
            std::uint64_t max_frame_mem = get_max_frame_allocator_mem_usage();

            std::cout << "Memory manager info: \n" <<
            "Start free mem: " << (start / 1024 / 1024) << " MB (" << (start / 1024) << " KB)" << std::endl <<
            "Max mem usage: " << (max_mem / 1024 / 1024) << " MB (" << (max_mem / 1024) << " KB)" << std::endl <<
            "Max frame allocator mem usage: " << (max_frame_mem / 1024 / 1024) << " MB (" << (max_frame_mem / 1024) << " KB)" << std::endl;
        }
    };

    // allocators ------------------------------

    FrameAllocator* global_frame_allocator = new FrameAllocator(Settings::get_max_frame_allocator_mem_usage());

    // -----------------------------------------
    void refresh()
    {
        global_frame_allocator->deallocate_mem_block();
    }

    // получение доступной свободной памяти
    std::uint64_t get_device_free_mem()
    {
        #if defined(_WIN32) || defined(_WIN64)
            MEMORYSTATUSEX status;
            status.dwLength = sizeof(status);
            GlobalMemoryStatusEx(&status);
            return status.ullAvailPhys;
        #elif defined(__unix__)
            // TODO: сделать для unix пон
        #endif
    }
}

#endif //TEST00_MEMORYMANAGER_H
