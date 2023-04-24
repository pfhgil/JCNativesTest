// Created by stuka on 10.04.2023.
//

#pragma once

#ifndef NATIVECORE_POOLALLOCATOR_H
#define NATIVECORE_POOLALLOCATOR_H

#include <iostream>
#include <vector>

#include "IAllocator.h"
#include "../../Logging/Log.h"

namespace Core::Memory::Allocators
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

            Core::Logging::c_printf(Core::Logging::MessageType::MT_INFO,
                                    "Pool allocator mem block ptr: %p, "
                                    "end: %p, total byte size: %llu, "
                                    "max region byte size: %llu, total elements count: %llu",
                                    this->free_region, this->end_region,
                                    this->total_byte_size, this->max_region_byte_size, this->total_elements_count);
        };

    public:
        PoolAllocator(const size_t& byte_size, size_t max_region_byte_size) : IAllocator()
        {
            this->max_region_byte_size = max_region_byte_size;
            this->total_elements_count = byte_size / max_region_byte_size;

            allocate_mem_block(byte_size + this->total_elements_count);
        };

        ~PoolAllocator() override
        {
            free();
        }

        template <typename Obj>
        std::uint64_t get_region_mem_size_for_obj()
        {
            std::uint64_t obj_sz = sizeof(Obj);

            // высчитываем сколько нужно блоков под объект
            // тут такая формула потому что: допустим obj_sz у нас 96 и max_region_byte_size тоже 96. тогда при делении у нас получается 1.
            // далее мы прибавляем 1 (для того, если obj_sz не кратно max_region_byte_size) и у нас получается 2 региона выделяется.
            // хотя должен всего 1
            // наглядный пример
            // (96 - 1) / 96 + 1 = 1
            // (97 - 1) / 96 + 1 = 2
            // (95 - 1) / 96 + 1 = 1
            int m = (int) ((obj_sz - 1) / this->max_region_byte_size) + 1;
            // финальный размер, который нужно выделить (размер зависит от размера блока (max_region_byte_size) в pool allocator)

            std::uint64_t obj_region_mem_size = m * this->max_region_byte_size;
            //Core::Logging::c_printf(Core::Logging::MessageType::MT_WARNING, "m is: %llu, obj_sz: %llu, final region sz: %llu", m, obj_sz, obj_region_mem_size);
            return obj_region_mem_size;
        }

        template <typename Obj>
        bool has_mem_for_obj()
        {
            return (this->free_region + get_region_mem_size_for_obj<Obj>()) > this->end_region;
        }

        template <typename T, typename... Params>
        T* allocate(Params&&... params)
        {
            std::uint64_t obj_sz = sizeof(T);

            if(!this->start_region)
            {
                allocate_mem_block(this->total_byte_size);
            }

            std::uint64_t obj_region_mem_size = get_region_mem_size_for_obj<T>();

            if(this->free_region == this->end_region || (this->free_region + obj_region_mem_size) > this->end_region)
            {
                Core::Logging::c_printf(Core::Logging::MessageType::MT_ERROR,
                                        "Pool allocator out of bounds! The last pointer is returned. Last region: %p, next region: %p. Requested byte size: %llu",
                                        this->free_region, this->free_region->next, obj_sz);

                return reinterpret_cast<T*>(this->free_region);
            }

            region current_free_region = this->free_region;

            this->free_region->next = reinterpret_cast<region>(reinterpret_cast<char*>(current_free_region) + obj_region_mem_size);
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
                Core::Logging::c_printf(Core::Logging::MessageType::MT_ERROR,"Attempt to delete an object that is not related to the pool!");

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


#endif //NATIVECORE_POOLALLOCATOR_H
