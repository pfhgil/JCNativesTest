// Created by stuka on 10.04.2023.
//

#ifndef TEST00_ALLOCATORS_H
#define TEST00_ALLOCATORS_H

#pragma once

#include "../../ECS/Transformations/TransformComponent.h"

using Memory::Allocators::FrameAllocator;
using Memory::Allocators::PoolAllocator;

// 5 мегабайте на разные вычисления в стеках и тд
//FrameAllocator* global_frame_allocator = new FrameAllocator(1024 * 1024 * 10);
//PoolAllocator* transform_component_pool_allocator = new PoolAllocator(1024 * 1024 * 500, sizeof(ECS::Transformations::TransformComponent));

#endif //TEST00_ALLOCATORS_H
