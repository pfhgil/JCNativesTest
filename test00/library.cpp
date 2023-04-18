#include "library.h"
#include "glm/mat2x2.hpp"
#include "glm/mat3x3.hpp"
#include "glm/mat4x4.hpp"
#include "Sources/ECS/Transformations/TransformComponent.h"
#include <type_traits>
#include "Sources/Memory/sg_frame_ptr.h"
#include "Sources/Memory/sg_global_ptr.h"
#include "Sources/Memory/MemoryManager.h"

// tests

using Memory::Allocators::PoolAllocator;
using ECS::Transformations::TransformComponent;
using Memory::Allocators::FrameAllocator;
using Memory::sg_frame_ptr;
using Memory::sg_global_ptr;

std::vector<FrameAllocator*> allocatorsVec = std::vector<FrameAllocator*>();

void allocator_full_allocate()
{
    for(int i = 0; i < allocatorsVec[0]->get_total_byte_size() / sizeof(TransformComponent); i++)
    {
        allocatorsVec[0]->allocate<TransformComponent>();
    }

    std::cout << "free region_ptr: " << allocatorsVec[0]->get_free_region() << ", end region_ptr: " << allocatorsVec[0]->get_end_region()
              << ", free size: " << allocatorsVec[0]->get_free_byte_size() << ", elements count: " << allocatorsVec[0]->get_elements_count() << std::endl;
}

void allocator_dealloc()
{
    allocatorsVec[0]->deallocate_mem_block();

    std::cout << "allocator deallocated" << std::endl;
}

void allocator_free()
{
    delete allocatorsVec[0];

    std::cout << "allocator free" << std::endl;
}

int main()
{
    Memory::MemoryManager::Settings::print_info();

    while(true)
    {
        try
        {
            //TransformComponent* t = global_frame_allocator->allocate<TransformComponent>();
            //sg_frame_ptr<TransformComponent> sgPtr = sg_frame_ptr<TransformComponent>();
            for(int i = 0; i < 1024 * 1024 / 36; i++)
            {
                //sg_global_ptr<TransformComponent> sgGlobalPtr = sg_global_ptr<TransformComponent>();
                sg_frame_ptr<TransformComponent> sgPtr = sg_frame_ptr<TransformComponent>();
            }

            Memory::MemoryManager::refresh();
            //transform_component_pool_allocator->deallocate_mem_block();
            //global_frame_allocator->deallocate_mem_block();
        }
        catch (std::exception& e)
        {
            std::cout << e.what() << std::endl;
        }
    }

    return 0;
}