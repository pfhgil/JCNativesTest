#ifndef TEST00_TRANSFORMCOMPONENT_H
#define TEST00_TRANSFORMCOMPONENT_H

#pragma once
#include "../../../library.h"
#include "../../Memory/Allocators/Allocators.h"

using glm::vec3;

namespace ECS
{
    namespace Transformations
    {

        class TransformComponent
        {
        public:
            //vec3* position = vec3Allocator->allocate(1);
            //vec3* rotation = vec3Allocator->allocate(1);
            //vec3* scale = vec3Allocator->allocate(1);

            ~TransformComponent()
            {

            }
        };
    }
}


#endif //TEST00_TRANSFORMCOMPONENT_H
