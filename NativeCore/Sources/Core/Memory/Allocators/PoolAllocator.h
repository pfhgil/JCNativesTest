// Created by stuka on 10.04.2023.
//

#ifndef TEST00_POOLALLOCATOR_H
#define TEST00_POOLALLOCATOR_H

#include <iostream>
#include <vector>
#include "IAllocator.h"
//#include "../../Utils/Utils.h"

namespace Memory::Allocators
{
    class PoolAllocator : public IAllocator
    {
        typedef AllocatorRegion *region;

    protected:
        void allocate_mem_block(size_t byte_size)
        {
            this->total_byte_size = byte_size;

            this->free_region = reinterpret_cast<region>(malloc(this->total_byte_size));

            this->start_region = this->free_region;

            // old version
            /*
            region current_region = this->free_region;

            for(int i = 0; i < this->total_elements_count; i++)
            {
                // reinterpret_cast<char*>(current_region) используется, чтобы к адресу прибавилось именно значение max_region_byte_size
                // например наш изначальный адрес был 0x01 мы хотим прибавить к адресу 8 байт, чтобы получить 0x09
                // если не сделать reinterpret_cast<char*>(current_region), то к нашему адресу прибавится sizeof(тип указателя (например double)) * max_region_byte_size
                // поэтому мы переводим в указатель на char, чтобы к нашему адресу прибавлялось sizeof(char) * max_region_byte_size
                // sizeof(char) - 1 байт
                current_region->next = reinterpret_cast<region>(reinterpret_cast<char*>(current_region) + this->max_region_byte_size);
                current_region = current_region->next;
            }

            current_region->next = nullptr;
            this->end_region = current_region;
             */

            // reinterpret_cast<char*>(current_region) используется, чтобы к адресу прибавилось именно значение max_region_byte_size
            // например наш изначальный адрес был 0x01 мы хотим прибавить к адресу 8 байт, чтобы получить 0x09
            // если не сделать reinterpret_cast<char*>(current_region), то к нашему адресу прибавится sizeof(тип указателя (например double)) * max_region_byte_size
            // поэтому мы переводим в указатель на char, чтобы к нашему адресу прибавлялось sizeof(char) * max_region_byte_size
            // sizeof(char) - 1 байт
            this->end_region = reinterpret_cast<region>(reinterpret_cast<char*>(this->start_region) + this->total_byte_size);

            std::cout << "pool allocator mem block ptr: " << this->free_region << ", end: " << this->end_region
                      << ". pool allocator size: " << this->total_byte_size << ". max region_ptr size: " << this->max_region_byte_size << ". count: "
                      << this->total_elements_count << std::endl;
        };

    public:
        PoolAllocator(const size_t& byte_size, size_t max_region_byte_size) : IAllocator()
        {
            this->max_region_byte_size = max_region_byte_size;
            this->total_elements_count = byte_size / max_region_byte_size;

            std::cout << "creating pool allocator..." << std::endl;
            std::cout << "pool allocator elements cnt: " << this->total_elements_count << std::endl;
            allocate_mem_block(byte_size + this->total_elements_count);
        };

        ~PoolAllocator() override
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

            if(this->free_region == this->end_region || (this->free_region + sizeof(T)) > (this->free_region + this->max_region_byte_size))
            {
                std::cout << "Error: Pool allocator out of bounds! The last pointer is returned. Before: "
                          << this->free_region << ", next: " << this->free_region->next << std::endl;

                return reinterpret_cast<T*>(this->free_region);
            }

            region current_free_region = this->free_region;

            this->free_region->next = reinterpret_cast<region>(reinterpret_cast<char*>(current_free_region) + this->max_region_byte_size);
            this->free_region = this->free_region->next;

            /*
            std::cout << "pool allocator allocated chunk: " << current_free_region << ", next: "
            << current_free_region->next << ", elements count: " << this->elements_count << ", end: "
            << this->end_region << std::endl;
             */

            this->elements_count++;

            return new(reinterpret_cast<T*>(current_free_region))T(std::forward<Params>(params)...);
        };

        template <typename T>
        void deallocate(T* obj_ptr)
        {
            auto obj_region = reinterpret_cast<region>(obj_ptr);

            if(obj_region < this->start_region || obj_region > this->end_region)
            {
                std::cout << "Error: Attempt to delete an object that is not related to the pool" << std::endl;

                return;
            }

            // пример (1 - занято, 0 - не занято). то, что в скобках - next
            // 1, 1, 1, 1, 0, 0
            // деаллоцируем объект в начале
            // 0, 1, 1, 1, 0, 0
            // аллоцируем новый объект
            // 1, 1, 1, 1, 0, 0
            // аллоцируем еще один объект
            // 1, 1, 1, 1, 1, 0

            // пример с удаление двух объектов
            // 1, 1, 1, 1, 0, 0
            // деаллоцируем один объект
            // 0, 1, 1, 1, 0, 0
            // затем второй
            // 0, 1, 1, 0, 0, 0
            // аллоцируем заново первый объект
            // 0, 1, 1, 1, 0, 0
            // второй
            // 1, 1, 1, 1, 0, 0
            // крч все возвращается так, как было
            obj_region->next = this->free_region;

            this->free_region = obj_region;

            //std::cout << "pool allocator current chunk (after deallocate): " << this->free_region << std::endl;

            this->elements_count--;
        };

        void deallocate_mem_block()
        {
            this->free_region = this->start_region;
            this->elements_count = 0;
        }
    };
}


#endif //TEST00_POOLALLOCATOR_H
