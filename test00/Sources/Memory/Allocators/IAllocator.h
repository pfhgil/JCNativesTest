//
// Created by stuka on 12.04.2023.
//

#ifndef TEST00_IALLOCATOR_H
#define TEST00_IALLOCATOR_H

#pragma once
#include <iostream>
#include "../../Utils/Utils.h"

namespace Memory::Allocators
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
            size_t start_pagefile_usage;
            size_t start_resident_set;

            size_t end_pagefile_usage;
            size_t end_resident_set;

            Utils::get_megabyte_process_mem_usage(start_pagefile_usage, start_resident_set);

            std::cout << "before delete allocator size (start_pagefile_usage): " << start_pagefile_usage << ", resident: " << start_resident_set
                      << std::endl;

            delete this->start_region;

            Utils::get_megabyte_process_mem_usage(end_pagefile_usage, end_resident_set);

            std::cout << "after delete allocator size (end_pagefile_usage): " << end_pagefile_usage << ", resident: " << end_resident_set
                      << std::endl;

            size_t pagefile_dif = start_pagefile_usage - end_pagefile_usage;
            size_t res_dif = start_resident_set - end_resident_set;

            std::cout << "dif vm: " << pagefile_dif << ", set: " << res_dif << std::endl;
        }

        //virtual void allocate_mem_block(size_t byte_size) = 0;

    public:
        IAllocator() = default;

        virtual ~IAllocator() = default;

        //template <typename T, typename... Params>
        //T* allocate(Params&&...) {};

        //virtual void deallocate(void* obj_ptr) = 0;

        //virtual void free() = 0;


        [[nodiscard]] inline size_t get_total_elements_count() const noexcept { return this->total_elements_count; }

        [[nodiscard]] inline size_t get_elements_count() const noexcept { return this->elements_count; }

        [[nodiscard]] inline size_t get_total_byte_size() const noexcept { return this->total_byte_size; }

        [[nodiscard]] inline region_ptr get_free_region() const noexcept { return this->free_region; }

        [[nodiscard]] inline region_ptr get_start_region() const noexcept { return this->start_region; }

        [[nodiscard]] inline region_ptr get_end_region() const noexcept { return this->end_region; }
    };
}


#endif //TEST00_IALLOCATOR_H
