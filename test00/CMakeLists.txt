cmake_minimum_required(VERSION 3.25)
project(test00)

set(CMAKE_CXX_STANDARD 23)

find_package(JNI REQUIRED)
find_package(Java REQUIRED)
include(UseJava)

if (JNI_FOUND)
    message (STATUS "JNI_INCLUDE_DIRS=${JNI_INCLUDE_DIRS}")
    message (STATUS "JNI_LIBRARIES=${JNI_LIBRARIES}")
endif()


set(CMAKE_SHARED_LINKER_FLAGS "-static-libgcc -static-libstdc++ -static")

set(glm_DIR libs/glm/cmake/glm)
find_package(glm REQUIRED)

file(GLOB_RECURSE sources "*.h" "*.cpp" ".hpp")

include_directories(${JNI_INCLUDE_DIRS} ${_classDir} ${_stubDir})
add_library(test00 SHARED ${sources})
target_link_libraries(test00 glm::glm)

add_executable(test01 library.cpp)
target_link_libraries(test01 glm::glm)