//
// Created by stuka on 16.04.2023.
//

#pragma once

#ifndef NATIVECORE_SG_GLOBAL_PTR_H
#define NATIVECORE_SG_GLOBAL_PTR_H

#include "MemoryManager.h"
#include <vector>

namespace Core::Memory
{
    // класс, содержащий в себе указатель на объект в како-либо pool allocator
    template <typename T>
    class sg_global_ptr
    {
        typedef T* ptr_t;

    private:
        // сам объект
        ptr_t m_Obj;

        // аллокатор, который обеспечил память для объекта m_Obj
        PoolAllocator* mem_provider = nullptr;

        void find_allocator()
        {
            // TODO: сделать распределение памяти умнее (по разным аллокаторам)
            using MemSettings = Core::Memory::MemoryManager::Settings;

            #ifndef OPTIMAL_VERSION
                const std::uint64_t obj_size = sizeof(T);

                const std::uint64_t dif_for__s_alloc = MemoryManager::global_small_objects_pool_allocator->get_region_mem_size_for_obj<T>() - obj_size;
                const std::uint64_t dif_for__m_alloc = MemoryManager::global_medium_objects_pool_allocator->get_region_mem_size_for_obj<T>() - obj_size;
                const std::uint64_t dif_for__b_alloc = MemoryManager::global_big_objects_pool_allocator->get_region_mem_size_for_obj<T>() - obj_size;

                Core::Logging::c_printf(Core::Logging::MessageType::MT_SUCCESS, "dif_for__s_alloc: %llu, dif_for__m_alloc: %llu, dif_for__b_alloc: %llu",
                                        dif_for__s_alloc, dif_for__m_alloc, dif_for__b_alloc);


                if(dif_for__b_alloc <= MemSettings::get__b_max_region_obj_dif())
                {

                    mem_provider = MemoryManager::global_big_objects_pool_allocator;
                }
                else if(dif_for__m_alloc <= MemSettings::get__m_max_region_obj_dif())
                {
                    //m_Obj = MemoryManager::global_medium_objects_pool_allocator->allocate<T>(params...);
                    mem_provider = MemoryManager::global_medium_objects_pool_allocator;
                }
                else if(dif_for__s_alloc <= MemSettings::get__s_max_region_obj_dif())
                {
                    //m_Obj = MemoryManager::global_small_objects_pool_allocator->allocate<T>(params...);
                    mem_provider = MemoryManager::global_small_objects_pool_allocator;
                }
                else
                {
                    Core::Logging::c_printf(Core::Logging::MessageType::MT_ERROR,
                                            "Core out of memory! Allocation is not possible. Try to change the memory usage factor by the Core");
                }
            #endif
        }

    public:
        // конструктор для объекта и выделение памяти. сохраняется ссылка на аллокатор, где был создан объект
        template<typename... Params>
        explicit sg_global_ptr(Params&&... params)
        {
            mem_provider = MemoryManager::global_small_objects_pool_allocator;
            m_Obj = mem_provider->allocate<T>(params...);
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

#endif //NATIVECORE_SG_GLOBAL_PTR_H
