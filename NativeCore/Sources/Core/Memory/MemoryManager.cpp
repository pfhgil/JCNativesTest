#include "MemoryManager.h"

// получение доступной свободной памяти
std::uint64_t Core::Memory::MemoryManager::get_device_free_mem()
{
    #if defined(_WIN32) || defined(_WIN64)
    MEMORYSTATUSEX status;
    status.dwLength = sizeof(status);
    GlobalMemoryStatusEx(&status);
    return status.ullAvailPhys;
    #elif defined(__unix__)
    // TODO: сделать для unix пон
    #endif
}

void Core::Memory::MemoryManager::init()
{
    Settings::print_info();

    global_frame_allocator = new FrameAllocator(Settings::get_max_frame_allocator_mem_usage());

    global_small_objects_pool_allocator = new PoolAllocator(Settings::get__s_max_objects_pool_allocator_mem_usage(), Settings::get__s_object_max_size()
    );

    #ifndef OPTIMAL_VERSION
        global_medium_objects_pool_allocator = new PoolAllocator(Settings::get__m_max_objects_pool_allocator_mem_usage(), Settings::get__m_object_max_size()
        );

        global_big_objects_pool_allocator = new PoolAllocator(Settings::get__b_max_objects_pool_allocator_mem_usage(), Settings::get__b_object_max_size()
        );
    #endif
}

void Core::Memory::MemoryManager::refresh()
{
    global_frame_allocator->deallocate_mem_block();
}