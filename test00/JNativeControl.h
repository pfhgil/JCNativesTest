#ifndef TEST00_JNATIVECONTROL_H
#define TEST00_JNATIVECONTROL_H

#include "library.h"
#include "Sources/Memory/Allocators/IAllocator.h"

#ifndef _Included_org_example_NativeControl
#define _Included_org_example_NativeControl
#ifdef __cplusplus
extern "C" {
#endif

JNIEXPORT void JNICALL Java_org_example_NativeControl_freeVec3Allocator(JNIEnv *, jclass, jlong jpoolAllocatorPtr)
{
    reinterpret_cast<Memory::Allocators::PoolAllocator*>(jpoolAllocatorPtr)->free();
    //vec3PoolAllocator->free();
    //
}

#ifdef __cplusplus
}
#endif
#endif

#endif //TEST00_JNATIVECONTROL_H
