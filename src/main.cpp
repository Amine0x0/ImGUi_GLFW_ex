
#include <GLFW/glfw3.h>

#include "App/Window/Window.h"

int main() {
    if (!glfwInit())
        return 1;

    APP::Main_Window* window_main = APP::Window::Window_running();
    APP::Window::cleanup(window_main);
    return 0;
}