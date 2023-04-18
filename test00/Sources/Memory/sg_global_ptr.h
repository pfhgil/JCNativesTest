//
// Created by stuka on 16.04.2023.
//

#ifndef TEST00_SG_GLOBAL_PTR_H
#define TEST00_SG_GLOBAL_PTR_H

#include "MemoryManager.h"

namespace Memory
{
    template <typename T>
    class sg_global_ptr
    {
        typedef T *ptr_t;

    private:
        ptr_t m_Obj;

    public:
        template<typename... Params>
        explicit sg_global_ptr(Params&&... params)
        {
            //m_Obj = transform_component_pool_allocator->allocate<T>(params...);
            //std::alloc
        }

        inline ~sg_global_ptr()
        {
            //transform_component_pool_allocator->deallocate(m_Obj);
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
