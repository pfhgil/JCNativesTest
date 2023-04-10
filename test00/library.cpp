#pragma once
#include "library.h"
using namespace std;

void set_float_field(JNIEnv*& env, jobject& obj, const string& fieldName, const jfloat& val)
{
    jclass cls = env->GetObjectClass(obj);
    jfieldID fieldID = env->GetFieldID(cls, fieldName.c_str(), "F");
    env->SetFloatField(obj, fieldID, val);
}
