#ifndef TEST00_TRANSFORMCOMPONENT_H
#define TEST00_TRANSFORMCOMPONENT_H

#pragma once
#include "../../../library.h"
#include "glm/glm.hpp"
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

        glm::mat4 translation_mat = glm::f32mat4();
        glm::mat4 rotation_mat = glm::f32mat4();
        glm::mat4 scale_mat = glm::f32mat4();

        glm::mat4 model_mat = glm::f32mat4();

        TransformComponent() = default;
        explicit TransformComponent(int d) {}

        ~TransformComponent()
        {

        }
    };
}


#endif //TEST00_TRANSFORMCOMPONENT_H
