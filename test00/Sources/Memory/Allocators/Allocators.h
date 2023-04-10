//
// Created by stuka on 10.04.2023.
//

#ifndef TEST00_ALLOCATORS_H
#define TEST00_ALLOCATORS_H

#pragma once
#include "../../../library.h"
//#include "StackAllocator.h"
#include "Allocators.h"
#include "PoolAllocator.h"

//const Memory::Allocators::StackAllocator<glm::vec3> vec3StackAllocator = Memory::Allocators::StackAllocator<glm::vec3>(1024);
Memory::Allocators::PoolAllocator* vec3PoolAllocator = new Memory::Allocators::PoolAllocator(10);
//std::allocator<glm::vec3>* vec3Allocator = new std::allocator<glm::vec3>();

#endif //TEST00_ALLOCATORS_H
