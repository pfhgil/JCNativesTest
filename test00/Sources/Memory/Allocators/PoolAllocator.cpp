//
// Created by stuka on 10.04.2023.
//

#pragma once
#include "PoolAllocator.h"

using Memory::Allocators::PoolAllocator;
using Memory::Allocators::AllocatorChunk;

AllocatorChunk* PoolAllocator::allocate_mem_block(size_t obj_size)
{
    size_t mem_block_size = this->count * obj_size;

    chunk first_chunk = reinterpret_cast<chunk>(malloc(mem_block_size));

    chunk cur = first_chunk;

    for(int i = 0; i < this->count; i++)
    {
        // char используется как заголовок для блока, чтобы не было наложения друг на друга адресов
        cur->next = reinterpret_cast<chunk>(reinterpret_cast<char*>(cur) + obj_size);
        cur = cur->next;
    }

    cur->next = nullptr;

    std::cout << "pool allocator mem block ptr: " << first_chunk << ". pool allocator size: " << mem_block_size << ". obj size: " << obj_size << ". count: " << this->count << std::endl;

    return first_chunk;
}

void* PoolAllocator::allocate(size_t obj_size)
{
    if(this->current_free_chunk == nullptr)
    {
        this->current_free_chunk = allocate_mem_block(obj_size);
    }

    chunk free_chunk = this->current_free_chunk;

    this->current_free_chunk = this->current_free_chunk->next;

    std::cout << "pool allocator allocated chunk: " << free_chunk << std::endl;

    return free_chunk;
}

void PoolAllocator::deallocate(void *obj_mem_block)
{
    // пример (1 - занято, 0 - не занято). то, что в скобках - next
    // 1, 1, 1, 1, 0, 0
    // деаллоцируем объект в начале
    // 0, 1, 1, 1, 0, 0
    // аллоцируем новый объект
    // 1, 1, 1, 1, 0, 0
    // аллоцируем еще один объект
    // 1, 1, 1, 1, 1, 0

    // пример с удаление двух объектов
    // 1, 1, 1, 1, 0, 0
    // деаллоцируем один объект
    // 0, 1, 1, 1, 0, 0
    // затем второй
    // 0, 1, 1, 0, 0, 0
    // аллоцируем заново первый объект
    // 0, 1, 1, 1, 0, 0
    // второй
    // 1, 1, 1, 1, 0, 0
    // крч все возвращается так, как было
    reinterpret_cast<chunk>(obj_mem_block)->next = this->current_free_chunk;

    this->current_free_chunk = reinterpret_cast<chunk>(obj_mem_block);

    std::cout << "pool allocator current chunk (after deallocate): " << this->current_free_chunk << std::endl;
}
