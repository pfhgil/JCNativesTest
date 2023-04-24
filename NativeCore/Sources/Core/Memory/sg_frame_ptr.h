//
// Created by stuka on 16.04.2023.
//

#pragma once

#ifndef NATIVECORE_SG_FRAME_PTR_H
#define NATIVECORE_SG_FRAME_PTR_H

#include "MemoryManager.h"

namespace Core::Memory
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

        /*
        template<typename ByteSize, typename... Params>
        explicit sg_frame_ptr(ByteSize byte_size, Params&&... params)
        {
            m_Obj = MemoryManager::global_frame_allocator->allocate<T, std::uint64_t>(byte_size, params...);
        }
         */

        inline ptr_t operator->() noexcept
        { return m_Obj; }

        inline T& operator*()
        { return *m_Obj; }
    };
}

#endif //NATIVECORE_SG_FRAME_PTR_H
