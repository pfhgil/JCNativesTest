#pragma once
#include "library.h"
#include "Sources//Memory/Allocators/Allocators.h"
#include <tuple>


#ifndef _Included_org_example_Main
#define _Included_org_example_Main
#ifdef __cplusplus
extern "C" {
#endif

std::tuple<glm::vec3*, jlong> curPtr(nullptr, NULL);


JNIEXPORT jlong JNICALL Java_org_example_Main_00024Vector3f_create(JNIEnv *, jobject)
{
    auto* newVec3 = reinterpret_cast<glm::vec3*>(vec3PoolAllocator->allocate(sizeof(glm::vec3)));

    return reinterpret_cast<jlong>(newVec3);
}

JNIEXPORT void JNICALL Java_org_example_Main_00024Vector3f_n_1memFree(JNIEnv* env, jobject calledFrom, jlong ptr)
{
    auto* vecPtr = reinterpret_cast<glm::vec3*>(ptr);

    vec3PoolAllocator->deallocate(vecPtr);
}

JNIEXPORT jfloat JNICALL Java_org_example_Main_00024Vector3f_n_1x__J(JNIEnv* env, jobject calledFrom, jlong ptr)
{
    return reinterpret_cast<glm::vec3*>(ptr)->x;
}

JNIEXPORT void JNICALL Java_org_example_Main_00024Vector3f_n_1x__JF(JNIEnv* env, jobject calledFrom, jlong ptr, jfloat val)
{
    reinterpret_cast<glm::vec3*>(ptr)->x = val;
}

JNIEXPORT jfloat JNICALL Java_org_example_Main_00024Vector3f_n_1y__J(JNIEnv* env, jobject calledFrom, jlong ptr)
{
    return reinterpret_cast<glm::vec3*>(ptr)->y;
}

JNIEXPORT void JNICALL Java_org_example_Main_00024Vector3f_n_1y__JF(JNIEnv* env, jobject calledFrom, jlong ptr, jfloat val)
{
    reinterpret_cast<glm::vec3*>(ptr)->y = val;
}

JNIEXPORT jfloat JNICALL Java_org_example_Main_00024Vector3f_n_1z__J(JNIEnv* env, jobject calledFrom, jlong ptr)
{
    return reinterpret_cast<glm::vec3*>(ptr)->z;
}

JNIEXPORT void JNICALL Java_org_example_Main_00024Vector3f_n_1z__JF(JNIEnv* env, jobject calledFrom, jlong ptr, jfloat val)
{
    reinterpret_cast<glm::vec3*>(ptr)->z = val;
}

JNIEXPORT void JNICALL Java_org_example_Main_00024Vector3f_doCrazy(JNIEnv *, jobject)
{
    int k = 0;
    for(int i = 0; i < 100000; i++)
    {
        k += i;
    }
}

JNIEXPORT void JNICALL Java_org_example_Main_00024Vector3f_n_1x_1y_1z(JNIEnv* env, jobject calledFrom, jlong ptr, jfloat x, jfloat y, jfloat z)
{
    auto* vecPtr = reinterpret_cast<glm::vec3*>(ptr);

    vecPtr->x = x;
    vecPtr->y = y;
    vecPtr->z = z;
}

#ifdef __cplusplus
}
#endif
#endif
