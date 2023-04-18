//
// Created by stuka on 13.04.2023.
//

#ifndef TEST00_JPOOLALLOCATOR_H
#define TEST00_JPOOLALLOCATOR_H

#pragma once
#include "library.h"
#include "Sources/Memory/Allocators/PoolAllocator.h"

#ifndef _Included_org_example_Main_PoolAllocator
#define _Included_org_example_Main_PoolAllocator
#ifdef __cplusplus
extern "C" {
#endif

JNIEXPORT jlong JNICALL Java_org_example_Main_00024PoolAllocator_create(JNIEnv* env, jobject calledFrom, jlong byteSize, jlong maxRegionByteSize)
{
    //auto* poolAllocatorPtr = new Memory::Allocators::PoolAllocator(byteSize, maxRegionByteSize);
    //return reinterpret_cast<jlong>(poolAllocatorPtr);
}

#ifdef __cplusplus
}
#endif
#endif

#endif //TEST00_JPOOLALLOCATOR_H
