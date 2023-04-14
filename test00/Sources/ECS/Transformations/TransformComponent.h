#ifndef TEST00_TRANSFORMCOMPONENT_H
#define TEST00_TRANSFORMCOMPONENT_H

#pragma once
#include "../../../library.h"
//#include "../../Memory/Allocators/Allocators.h"

//using glm::vec3;

namespace ECS::Transformations
{
    class TransformComponent
    {
    public:
        glm::vec3 position = glm::vec3();
        glm::vec3 rotation = glm::vec3();
        glm::vec3 scale = glm::vec3();

        TransformComponent() = default;

        ~TransformComponent()
        {

        }
    };
}


#endif //TEST00_TRANSFORMCOMPONENT_H
