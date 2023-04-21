//
// Created by stuka on 17.04.2023.
//

#ifndef TEST00_MEMORYMANAGER_H
#define TEST00_MEMORYMANAGER_H

#include "Allocators/FrameAllocator.h"
#include "Allocators/PoolAllocator.h"
#include <windows.h>
#include <unistd.h>

// defines

// ----------

using Memory::Allocators::FrameAllocator;
using Memory::Allocators::PoolAllocator;

namespace Memory::MemoryManager
{
    std::uint64_t get_device_free_mem();

    class Settings
    {
    private:
        // ВСЕ ПОКАЗАТЕЛИ В БАЙТАХ

        // максимальный коэффициент памяти (от свободной памяти), которое может использовать движок
        static inline float max_free_mem_usage_coeff = 0.5f;


        // кол-во свободной памяти на старте
        static inline std::uint64_t start_free_mem = get_device_free_mem();

        // максимальное кол-во используемой памяти
        static inline std::uint64_t max_core_mem_usage = start_free_mem * max_free_mem_usage_coeff;

        // allocators settings ---------------------
        // максимальный коэффициент памяти, который может использовать frame allocator (от максимальной занимаемой движком памяти)
        static inline float max_frame_allocator_mem_usage_coeff = 0.01f;
        static inline std::uint64_t max_frame_allocator_mem_usage = max_core_mem_usage * max_frame_allocator_mem_usage_coeff;

        // настройки для pool аллокаторов ----------

        // размеры должны выставляться в последовательном порядке (small - medium - big)
        // желательно выставлять medium_object_max_size и big_object_max_size по формуле: small_object_max_size * <coeff>

        // максимальный размер маленького объекта - 96 байт
        static inline std::uint64_t small_object_max_size = 96;
        // максимальный размер среднего объекта - 96 * 8 (768) байт
        static inline std::uint64_t medium_object_max_size = small_object_max_size * 8;
        // максимальный размер большого объекта - 96 * 32 (3072) байт
        static inline std::uint64_t big_object_max_size = small_object_max_size * 32;

        // максимальный коэффициент памяти, который могут использовать разные pool allocator (от максимальной занимаемой движком памяти)
        static inline float max_small_objects_pool_allocator_mem_usage_coeff = 0.09;
        static inline float max_medium_objects_pool_allocator_mem_usage_coeff = 0.3;
        static inline float max_big_objects_pool_allocator_mem_usage_coeff = 0.6;

        // максимальный объем памяти, который могут использовать разные pool allocator (от максимальной занимаемой движком памяти)
        static inline std::uint64_t max_small_objects_pool_allocator_mem_usage =
                max_core_mem_usage * max_small_objects_pool_allocator_mem_usage_coeff;
        static inline std::uint64_t max_medium_objects_pool_allocator_mem_usage =
                max_core_mem_usage * max_medium_objects_pool_allocator_mem_usage_coeff;
        static inline std::uint64_t max_big_objects_pool_allocator_mem_usage =
                max_core_mem_usage * max_big_objects_pool_allocator_mem_usage_coeff;

    public:
        static inline std::uint64_t get_start_free_mem() noexcept { return start_free_mem; }
        static inline std::uint64_t get_max_mem_usage() noexcept { return max_core_mem_usage; }

        static inline std::uint64_t get_max_frame_allocator_mem_usage() noexcept { return max_frame_allocator_mem_usage; }

        static inline std::uint64_t get_max_small_objects_pool_allocator_mem_usage() noexcept { return max_small_objects_pool_allocator_mem_usage; }
        static inline std::uint64_t get_max_medium_objects_pool_allocator_mem_usage() noexcept { return max_medium_objects_pool_allocator_mem_usage; }
        static inline std::uint64_t get_max_big_objects_pool_allocator_mem_usage() noexcept { return max_big_objects_pool_allocator_mem_usage; }

        static inline std::uint64_t get_small_object_max_size() noexcept { return small_object_max_size; }
        static inline std::uint64_t get_medium_object_max_size() noexcept { return medium_object_max_size; }
        static inline std::uint64_t get_big_object_max_size() noexcept { return big_object_max_size; }

        static void print_info() noexcept
        {
            std::uint64_t start = get_start_free_mem();
            std::uint64_t max_mem = get_max_mem_usage();

            std::uint64_t max_frame_mem = get_max_frame_allocator_mem_usage();

            std::uint64_t max_small_mem = get_max_small_objects_pool_allocator_mem_usage();
            std::uint64_t max_medium_mem = get_max_medium_objects_pool_allocator_mem_usage();
            std::uint64_t max_big_mem = get_max_big_objects_pool_allocator_mem_usage();

            std::uint64_t max_small_objects = max_small_mem / get_small_object_max_size();
            std::uint64_t max_medium_objects = max_medium_mem / get_medium_object_max_size();
            std::uint64_t max_big_objects = max_big_mem / get_big_object_max_size();

            printf("Memory manager info:\n");
            printf("Start free mem: %llu MB (%llu KB)\n", (start / 1024 / 1024), (start / 1024));
            printf("Max mem usage: %llu MB (%llu KB)\n", (max_mem / 1024 / 1024), (max_mem / 1024));

            printf("Max frame allocator mem usage: %llu MB (%llu KB)\n", (max_frame_mem / 1024 / 1024), (max_frame_mem / 1024));

            printf("Max pool allocator (small objects) mem usage: %llu MB (%llu KB) (%llu objects)\n",
                   (max_small_mem / 1024 / 1024), (max_small_mem / 1024), max_small_objects);
            printf("Max pool allocator (medium objects) mem usage: %llu MB (%llu KB) (%llu objects)\n",
                   (max_medium_mem / 1024 / 1024), (max_medium_mem / 1024), max_medium_objects);
            printf("Max pool allocator (big objects) mem usage: %llu MB (%llu KB) (%llu objects)\n",
                   (max_big_mem / 1024 / 1024), (max_big_mem / 1024), max_big_objects);

            printf("--------------------------------------------------------------\n");
        }
    };

    // allocators ------------------------------

    FrameAllocator* global_frame_allocator;

    PoolAllocator* global_small_objects_pool_allocator;
    PoolAllocator* global_medium_objects_pool_allocator;
    PoolAllocator* global_big_objects_pool_allocator;

    // -----------------------------------------
    void init()
    {
        Settings::print_info();

        global_frame_allocator = new FrameAllocator(Settings::get_max_frame_allocator_mem_usage());

        global_small_objects_pool_allocator = new PoolAllocator(
                Settings::get_max_small_objects_pool_allocator_mem_usage(), Settings::get_small_object_max_size()
                );

        global_medium_objects_pool_allocator = new PoolAllocator(
                Settings::get_max_medium_objects_pool_allocator_mem_usage(), Settings::get_medium_object_max_size()
        );

        global_big_objects_pool_allocator = new PoolAllocator(
                Settings::get_max_big_objects_pool_allocator_mem_usage(), Settings::get_big_object_max_size()
        );
    }

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
