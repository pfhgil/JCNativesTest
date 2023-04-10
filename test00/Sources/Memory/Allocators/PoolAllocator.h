//
// Created by stuka on 10.04.2023.
//

#ifndef TEST00_POOLALLOCATOR_H
#define TEST00_POOLALLOCATOR_H

#pragma once
#include <iostream>
#include <vector>

namespace Memory
{
    namespace Allocators
    {
        struct AllocatorChunk
        {
            AllocatorChunk* next;
        };

        class PoolAllocator
        {
            typedef AllocatorChunk* chunk;

            typedef void* ptr;
            typedef const void* const_ptr;

        private:
            chunk current_free_chunk = nullptr;

            size_t count;
            //int chunk_byte_size = sizeof(T);

            chunk allocate_mem_block(size_t obj_size);
            void deallocate_mem_block(ptr block);

        public:
            PoolAllocator(size_t cnt) : count(cnt)
            {
                std::cout << "Pool allocator created! cnt: " << this->count << std::endl;
            };

            void* allocate(size_t obj_size);
            void deallocate(void* obj_mem_block);
        };
    }
}


#endif //TEST00_POOLALLOCATOR_H
