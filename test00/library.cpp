#include "library.h"
#include "glm/mat2x2.hpp"
#include "glm/mat3x3.hpp"
#include "glm/mat4x4.hpp"
#include "Sources/ECS/Transformations/TransformComponent.h"

// tests

using Memory::Allocators::PoolAllocator;
using ECS::Transformations::TransformComponent;
using Memory::Allocators::FrameAllocator;

int main()
{
    // for waiting
    std::string s;

    //std::cin >> s;

    //std::vector<PoolAllocator*> allocatorsVec = std::vector<PoolAllocator*>();
    std::vector<FrameAllocator*> allocatorsVec = std::vector<FrameAllocator*>();
    //allocatorsVec.push_back(new PoolAllocator(1024 * 1024 * 1000, sizeof(TransformComponent)));
    allocatorsVec.push_back(new FrameAllocator(1024 * 1024 * 1000 + 32));

    std::cout << "allocating..." << std::endl;

    for(int i = 0; i < allocatorsVec[0]->get_total_byte_size() / sizeof(TransformComponent); i++)
    {
        allocatorsVec[0]->allocate<TransformComponent>();
    }

    std::cout << "free region_ptr: " << allocatorsVec[0]->get_free_region() << ", end region_ptr: " << allocatorsVec[0]->get_end_region()
    << ", free size: " << allocatorsVec[0]->get_free_byte_size() << ", elements count: " << allocatorsVec[0]->get_elements_count() << std::endl;

    std::cout << "allocated" << std::endl;

    //std::cin >> s;

    delete allocatorsVec[0];

    //std::cin >> s;

    return 0;
}