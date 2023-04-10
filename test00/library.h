#ifndef TEST00_LIBRARY_H
#define TEST00_LIBRARY_H

#pragma once
#include <jni.h>
#include <cstdio>
#include <iostream>

#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>

void set_float_field(JNIEnv*& env, jobject& obj, const std::string& fieldName, const jfloat& val);

#include "java_methods.h"
//#include "java_methods.h"

#endif //TEST00_LIBRARY_H
