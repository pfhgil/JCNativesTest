//
// Created by stuka on 16.04.2023.
//

#ifndef TEST00_SG_GLOBAL_PTR_H
#define TEST00_SG_GLOBAL_PTR_H

#include "MemoryManager.h"

namespace Memory
{
    // класс, содержащий в себе указатель на объект в како-либо pool allocator
    template <typename T>
    class sg_global_ptr
    {
        typedef T *ptr_t;

    private:
        // сам объект
        ptr_t m_Obj;

        // аллокатор, который обеспечил память для объекта m_Obj
        PoolAllocator* mem_provider = nullptr;

    public:
        // конструктор для объекта и выделение памяти. сохраняется ссылка на аллокатор, где был создан объект
        template<typename... Params>
        explicit sg_global_ptr(Params&&... params)
        {
            std::uint64_t obj_size = sizeof(T);

            using MemSettings = MemoryManager::Settings;

            // аллокация для маленьких объектов
            if(obj_size > 0 && obj_size <= MemSettings::get_small_object_max_size())
            {
                m_Obj = MemoryManager::global_small_objects_pool_allocator->allocate<T>(params...);
                mem_provider = MemoryManager::global_small_objects_pool_allocator;
            }
            // аллокация для средних объектов
            else if(obj_size > MemSettings::get_small_object_max_size() && obj_size <= MemSettings::get_medium_object_max_size())
            {
                m_Obj = MemoryManager::global_medium_objects_pool_allocator->allocate<T>(params...);
                mem_provider = MemoryManager::global_medium_objects_pool_allocator;
            }
            // аллокация для больших объектов
            else if(obj_size > MemSettings::get_medium_object_max_size() && obj_size <= MemSettings::get_big_object_max_size())
            {
                m_Obj = MemoryManager::global_big_objects_pool_allocator->allocate<T>(params...);
                mem_provider = MemoryManager::global_big_objects_pool_allocator;
            }
            else
            {
                printf("sg_global_ptr allocation error: object size is less then 1 byte or bigger then %llu\n", MemSettings::get_big_object_max_size());
            }
        }

        // деаллоцирование происходит также объекта в аллокаторе
        inline ~sg_global_ptr()
        {
            mem_provider->deallocate(m_Obj);
        }

        inline ptr_t operator->() noexcept
        {
            return m_Obj;
        }

        inline ptr_t &operator*()
        {
            return *m_Obj;
        }
    };
}

#endif //TEST00_SG_GLOBAL_PTR_H
