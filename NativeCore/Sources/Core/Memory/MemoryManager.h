//
// Created by stuka on 17.04.2023.
//

#pragma once

#ifndef NATIVECORE_MEMORYMANAGER_H
#define NATIVECORE_MEMORYMANAGER_H

#include "Allocators/FrameAllocator.h"
#include "Allocators/PoolAllocator.h"
#include "../Logging/Log.h"
#include <windows.h>
#include <unistd.h>

// defines
// оптимальное решение
#define OPTIMAL_VERSION
// ----------

using Core::Memory::Allocators::FrameAllocator;
using Core::Memory::Allocators::PoolAllocator;

namespace Core::Memory::MemoryManager
{
    std::uint64_t get_device_free_mem();


    /**
     * Prefix _b - big
     * Prefix _m - medium
     * Prefix _s - small
     */
    // TODO: сделать распределение памяти умнее (по разным аллокаторам)
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
        // желательно выставлять _m_object_max_size и _b_object_max_size по формуле: _s_object_max_size * <coeff>

        // максимальный размер маленького объекта - 96 байт
        static inline std::uint64_t _s_object_max_size = 96;
        // максимальный размер среднего объекта - 96 * 8 (768) байт
        static inline std::uint64_t _m_object_max_size = _s_object_max_size * 8;
        // максимальный размер большого объекта - 96 * 32 (3072) байт
        static inline std::uint64_t _b_object_max_size = _s_object_max_size * 32;

        // максимальный коэффициент памяти, который могут использовать разные pool allocator (от максимальной занимаемой движком памяти)
        #ifdef OPTIMAL_VERSION
            static inline float _s_max_objects_pool_allocator_mem_usage_coeff = 0.99f;
        #else
            static inline float _s_max_objects_pool_allocator_mem_usage_coeff = 0.09;
        #endif
        static inline float _m_max_objects_pool_allocator_mem_usage_coeff = 0.3;
        static inline float _b_max_objects_pool_allocator_mem_usage_coeff = 0.6;

        // максимальный объем памяти, который могут использовать разные pool allocator (от максимальной занимаемой движком памяти)
        static inline std::uint64_t max_small_objects_pool_allocator_mem_usage =
                max_core_mem_usage * _s_max_objects_pool_allocator_mem_usage_coeff;
        static inline std::uint64_t _m_max_objects_pool_allocator_mem_usage =
                max_core_mem_usage * _m_max_objects_pool_allocator_mem_usage_coeff;
        static inline std::uint64_t _b_max_objects_pool_allocator_mem_usage =
                max_core_mem_usage * _b_max_objects_pool_allocator_mem_usage_coeff;

        static inline std::uint64_t _s_max_region_obj_dif = _s_object_max_size * 0.1f;
        static inline std::uint64_t _m_max_region_obj_dif = _m_object_max_size * 0.1f;
        static inline std::uint64_t _b_max_region_obj_dif = _b_object_max_size * 0.1f;

    public:
        static inline std::uint64_t get_start_free_mem() noexcept { return start_free_mem; }
        static inline std::uint64_t get_max_mem_usage() noexcept { return max_core_mem_usage; }

        static inline std::uint64_t get_max_frame_allocator_mem_usage() noexcept { return max_frame_allocator_mem_usage; }

        static inline std::uint64_t get__s_max_objects_pool_allocator_mem_usage() noexcept { return max_small_objects_pool_allocator_mem_usage; }
        static inline std::uint64_t get__m_max_objects_pool_allocator_mem_usage() noexcept { return _m_max_objects_pool_allocator_mem_usage; }
        static inline std::uint64_t get__b_max_objects_pool_allocator_mem_usage() noexcept { return _b_max_objects_pool_allocator_mem_usage; }

        static inline std::uint64_t get__s_object_max_size() noexcept { return _s_object_max_size; }
        static inline std::uint64_t get__m_object_max_size() noexcept { return _m_object_max_size; }
        static inline std::uint64_t get__b_object_max_size() noexcept { return _b_object_max_size; }

        static inline std::uint64_t get__s_max_region_obj_dif() noexcept { return _s_max_region_obj_dif; }
        static inline std::uint64_t get__m_max_region_obj_dif() noexcept { return _m_max_region_obj_dif; }
        static inline std::uint64_t get__b_max_region_obj_dif() noexcept { return _b_max_region_obj_dif; }

        static void print_info() noexcept
        {
            using namespace Core::Logging;
            using Core::Logging::MessageType;

            std::uint64_t start = get_start_free_mem();
            std::uint64_t max_mem = get_max_mem_usage();

            std::uint64_t max_frame_mem = get_max_frame_allocator_mem_usage();

            std::uint64_t _s_max_mem = get__s_max_objects_pool_allocator_mem_usage();
            std::uint64_t _m_max_mem = get__m_max_objects_pool_allocator_mem_usage();
            std::uint64_t _b_max_mem = get__b_max_objects_pool_allocator_mem_usage();

            std::uint64_t _s_max_objects = _s_max_mem / get__s_object_max_size();
            std::uint64_t _m_max_objects = _m_max_mem / get__m_object_max_size();
            std::uint64_t _b_max_objects = _b_max_mem / get__b_object_max_size();

            c_printf(MT_INFO,  "--------------------------------------------------------------");
            c_printf(MT_INFO, "Memory manager info:\n");

            c_printf(MT_INFO,  "Start free mem: %llu MB (%llu KB)", (start / 1024 / 1024), (start / 1024));
            c_printf(MT_INFO, "Max mem usage: %llu MB (%llu KB)\n", (max_mem / 1024 / 1024), (max_mem / 1024));


            c_printf(MT_INFO, "Settings:");
            c_printf(MT_INFO, "Small object max size: %llu bytes", get__s_object_max_size());
            c_printf(MT_INFO, "Medium object max size: %llu bytes", get__m_object_max_size());
            c_printf(MT_INFO, "Big object max size: %llu bytes\n", get__b_object_max_size());

            c_printf(MT_INFO, "Small allocator max difference between object size and region size: %llu bytes", get__s_max_region_obj_dif());
            c_printf(MT_INFO, "Medium allocator max difference between object size and region size: %llu bytes", get__m_max_region_obj_dif());
            c_printf(MT_INFO, "Big allocator max difference between object size and region size: %llu bytes\n", get__b_max_region_obj_dif());

            c_printf(MT_INFO, "Allocators settings:");
            c_printf(MT_INFO, "Max frame allocator mem usage: %llu MB (%llu KB)", (max_frame_mem / 1024 / 1024), (max_frame_mem / 1024));

            c_printf(MT_INFO, "Max pool allocator (small objects) mem usage: %llu MB (%llu KB) (%llu objects)",
                     (_s_max_mem / 1024 / 1024), (_s_max_mem / 1024), _s_max_objects);
            #ifndef OPTIMAL_VERSION
                c_printf(MT_INFO, "Max pool allocator (medium objects) mem usage: %llu MB (%llu KB) (%llu objects)",
                         (_m_max_mem / 1024 / 1024), (_m_max_mem / 1024), _m_max_objects);
                c_printf(MT_INFO, "Max pool allocator (big objects) mem usage: %llu MB (%llu KB) (%llu objects)",
                         (_b_max_mem / 1024 / 1024), (_b_max_mem / 1024), _b_max_objects);
            #endif

            c_printf(MT_INFO, "--------------------------------------------------------------");
        }
    };

    // allocators ------------------------------

    inline FrameAllocator* global_frame_allocator;

    inline PoolAllocator* global_small_objects_pool_allocator;
    inline PoolAllocator* global_medium_objects_pool_allocator;
    inline PoolAllocator* global_big_objects_pool_allocator;

    // -----------------------------------------
    void init();

    void refresh();
}

#endif //NATIVECORE_MEMORYMANAGER_H
