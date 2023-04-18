//
// Created by stuka on 18.04.2023.
//

#include "CoreTest.h"
//#include <GLFW/glfw3.h>
#include <iostream>

GLFWwindow* wnd = nullptr;

void print_glfw_errors(int errc, const char* err)
{
    std::cerr << "GLFW error: " << err << std::endl;
}

int main()
{
    std::cout << "Hello test!" << std::endl;

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

    while(true)
    {
        glfwSwapBuffers(wnd);
        glfwPollEvents();
    }

    // ---------------------------------------------------

    return 0;
}