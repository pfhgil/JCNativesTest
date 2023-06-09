//
// Created by stuka on 12.04.2023.
//

#pragma once

#ifndef NATIVECORE_IALLOCATOR_H
#define NATIVECORE_IALLOCATOR_H

#include <iostream>

#include "../Utils.h"
#include "../../Logging/Log.h"

namespace Core::Memory::Allocators
{
    struct AllocatorRegion
    {
        AllocatorRegion *next;
    };

    //template <typename T>
    class IAllocator
    {
        typedef AllocatorRegion* region_ptr;

    protected:
        size_t total_byte_size = 0;
        size_t max_region_byte_size = 1;

        size_t elements_count = 0;
        size_t total_elements_count = 0;

        region_ptr free_region = nullptr;
        region_ptr start_region = nullptr;
        region_ptr end_region = nullptr;

        void free()
        {
            using namespace Core::Memory::Utils;

            size_t start_pagefile_usage;
            size_t start_resident_set;

            size_t end_pagefile_usage;
            size_t end_resident_set;

            get_megabyte_process_mem_usage(start_pagefile_usage, start_resident_set);

            delete this->start_region;

            get_megabyte_process_mem_usage(end_pagefile_usage, end_resident_set);

            size_t pagefile_dif = start_pagefile_usage - end_pagefile_usage;
            size_t res_dif = start_resident_set - end_resident_set;

            Core::Logging::c_printf(Core::Logging::MessageType::MT_INFO,"Allocator deallocated: %llu", res_dif);
        }

        //virtual void allocate_mem_block(size_t byte_size) = 0;

    public:
        IAllocator() = default;

        virtual ~IAllocator() = default;

        [[nodiscard]] inline size_t get_total_elements_count() const noexcept { return this->total_elements_count; }

        [[nodiscard]] inline size_t get_elements_count() const noexcept { return this->elements_count; }

        [[nodiscard]] inline size_t get_total_byte_size() const noexcept { return this->total_byte_size; }

        [[nodiscard]] inline region_ptr get_free_region() const noexcept { return this->free_region; }

        [[nodiscard]] inline region_ptr get_start_region() const noexcept { return this->start_region; }

        [[nodiscard]] inline region_ptr get_end_region() const noexcept { return this->end_region; }
    };
}


#endif //NATIVECORE_IALLOCATOR_H
