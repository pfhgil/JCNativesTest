//
// Created by stuka on 16.04.2023.
//

#ifndef TEST00_SG_FRAME_PTR_H
#define TEST00_SG_FRAME_PTR_H

#include "MemoryManager.h"

namespace Memory
{
    template <typename T>
    class sg_frame_ptr
    {
        typedef T *ptr_t;

    private:
        ptr_t m_Obj;

    public:
        template<typename... Params>
        explicit sg_frame_ptr(Params&&... params)
        {
            m_Obj = MemoryManager::global_frame_allocator->allocate<T>(params...);
        }

        inline ptr_t operator->() noexcept
        { return m_Obj; }

        inline ptr_t &operator*()
        { return *m_Obj; }
    };
}

#endif //TEST00_SG_FRAME_PTR_H
