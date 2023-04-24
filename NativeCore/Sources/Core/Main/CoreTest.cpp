//
// Created by stuka on 18.04.2023.
//

#include "CoreTest.h"
//#date <GLFW/glfw3.h>
#include <iostream>
#include "../Memory/MemoryManager.h"
#include "../Graphics/GraphicsManager.h"
#include "../Graphics/Api/Base.h"
#include "../Memory/sg_global_ptr.h"
#include "../Memory/sg_frame_ptr.h"

GLFWwindow* wnd = nullptr;

void print_glfw_errors(int errc, const char* err)
{
    std::cerr << "GLFW error: " << err << std::endl;
}

class Test
{
public:
    int t0 = 5;
    int t1 = 6;
    int t2 = 7;
};

int main()
{
    std::cout << "Hello test!" << std::endl;

    Core::Logging::init();
    Core::Memory::MemoryManager::init();

    // TODO: just test to delete! -----------------------------

    glfwSetErrorCallback(print_glfw_errors);
    if(!glfwInit())
    {
        print_glfw_errors(0, "Error initializing GLFW!");
    }

    glfwDefaultWindowHints(); // установка для будущего окна дефолтных настроек
    glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);

    wnd = glfwCreateWindow(500, 500, "telezhechka", NULL, NULL);

    glfwMakeContextCurrent(wnd);

    glfwSwapInterval(0);

    // сделать окно видимым
    glfwShowWindow(wnd);
    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

    using Core::Memory::sg_frame_ptr;
    using Core::Memory::sg_global_ptr;

    /*
    float res = 5000;
    for(int i = 0; i < 30 * 36; i++)
    {
        res += res * 0.01f;
        res += res * 0.0124f;

        printf("day %i result is: %f\n", i, res);
    }

    printf("result is: %f\n", res);

     */

    while(true)
    {
        glBegin(GL_TRIANGLES);

        glVertex2f(0, 0);
        glVertex2f(0, 0.5);
        glVertex2f(0.5, 0);

        glEnd();

        auto buf = sg_global_ptr<Core::Graphics::API::Base::IFrameBuffer>();

        Core::Memory::MemoryManager::refresh();

        glfwSwapBuffers(wnd);
        glfwPollEvents();
    }

    // ---------------------------------------------------

    return 0;
}