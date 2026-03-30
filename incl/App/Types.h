#ifndef APP_TYPES_H
#define APP_TYPES_H

#include <GLFW/glfw3.h>
#include "imgui.h"

namespace APP {
    typedef struct {
        GLFWwindow* window;
        ImVec4 clear_color;
        bool show_window_demo;
    } Main_Window;
}

#endif
