//
// Created by stuka on 14.04.2023.
//

#ifndef TEST00_FRAMEALLOCATOR_H
#define TEST00_FRAMEALLOCATOR_H

#include "IAllocator.h"

namespace Memory::Allocators
{
    class FrameAllocator : public IAllocator
    {
        typedef AllocatorRegion *region;

    protected:
        void allocate_mem_block(const size_t& byte_size)
        {
            this->total_byte_size = byte_size;

            this->free_region = reinterpret_cast<region>(malloc(this->total_byte_size));

            this->start_region = this->free_region;

            this->end_region = reinterpret_cast<region>(reinterpret_cast<char*>(this->start_region) + this->total_byte_size);

            std::cout << "frame allocator mem block ptr: " << this->free_region << ", end: " << this->end_region
                      << ". frame allocator size: " << this->total_byte_size << ", free byte size: " << get_free_byte_size() << std::endl;
        }

    public:
        explicit FrameAllocator(const size_t& byte_size)
        {
            //std::cout << "creating frame allocator..." << std::endl;
            allocate_mem_block(byte_size);
        }

        ~FrameAllocator() override
        {
            free();
        }

        template <typename T, typename... Params>
        T* allocate(Params&&... params)
        {
            if(!this->start_region)
            {
                allocate_mem_block(this->total_byte_size);
            }

            const size_t obj_byte_size = sizeof(T);

            if(this->free_region == this->end_region || obj_byte_size > get_free_byte_size())
            {
                std::cout << "Error: Frame allocator out of bounds! The last pointer is returned. Before: "
                          << this->free_region << ", next: " << this->free_region->next << ". Requested byte size: " << obj_byte_size<< ", free byte size: " << get_free_byte_size() <<
                          ", elements count: " << this->elements_count << std::endl;

                return reinterpret_cast<T*>(this->free_region);
            }

            region current_free_region = this->free_region;

            this->free_region->next = reinterpret_cast<region>(reinterpret_cast<char*>(current_free_region) + obj_byte_size);
            this->free_region = this->free_region->next;

            /*
            std::cout << "frame allocator allocated chunk: " << current_free_region << ", next: "
            << current_free_region->next << ", free size: " << get_free_byte_size() << ", obj size: "
            << obj_byte_size << ", end: " << this->end_region << std::endl;
             */

            this->elements_count++;

            return new(reinterpret_cast<T*>(current_free_region))T(std::forward<Params>(params)...);
        }

        void deallocate_mem_block()
        {
            this->free_region = this->start_region;
            this->elements_count = 0;
        }

        inline size_t get_free_byte_size()
        {
            return reinterpret_cast<char*>(this->end_region) - reinterpret_cast<char*>(this->free_region);
        }
    };
}


#endif //TEST00_FRAMEALLOCATOR_H
