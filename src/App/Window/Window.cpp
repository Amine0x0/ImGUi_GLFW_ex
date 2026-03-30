#include "App/Window/Window.h"
#include "App/Exceptions.h"
#include "App/GUI/Panels.h"
#include <iostream>
#include <cstdlib>

#include <GLFW/glfw3.h>
#include <GL/gl.h>

#include "imgui.h"
#include "imgui_impl_opengl3.h"
#include "imgui_impl_glfw.h"

namespace APP {
    namespace Window {
        [[noreturn]] inline void emergency_leave(){
            exit(0);
        }

        Main_Window* init_backend(){
            Main_Window* main = new Main_Window;

            GLFWwindow* window = glfwCreateWindow(1280, 800, "DEBUGGING WINDOW", nullptr, nullptr);
            if (!window)
                throw init_except();
            main->window = window;
            glfwMakeContextCurrent(window);
            glfwSwapInterval(1);
            IMGUI_CHECKVERSION();
            ImGui::CreateContext();

            ImGuiIO& io = ImGui::GetIO(); (void)io;
            io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
            io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;

            ImGui::StyleColorsDark();

            ImGui_ImplGlfw_InitForOpenGL(window, true);
            ImGui_ImplOpenGL3_Init("#version 130");

            ImVec4 clear_color = ImVec4(0.2f, 0.2f, 0.2f, 0.2f);
            main->clear_color = clear_color;

            bool show_demo_window = false;
            main->show_window_demo = show_demo_window;
            return main;
        }

        void WindowLoop(Main_Window* window_main){
            while (!glfwWindowShouldClose(window_main->window)) {
                glfwPollEvents();

                ImGui_ImplOpenGL3_NewFrame();
                ImGui_ImplGlfw_NewFrame();
                ImGui::NewFrame();

                APP::GUI::StatPanel();
                ImGui::Render();
                int display_w, display_h;
                glfwGetFramebufferSize(window_main->window, &display_w, &display_h);
                glViewport(0, 0, display_w, display_h);
                glClearColor(window_main->clear_color.x, window_main->clear_color.y, window_main->clear_color.z, window_main->clear_color.w);
                glClear(GL_COLOR_BUFFER_BIT);

                ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
                glfwSwapBuffers(window_main->window);
            }
            throw(aborted_window());
        }

        Main_Window* Window_running(){
            Main_Window* window_main{};
            try{
                window_main = init_backend();
            }
            catch (std::exception& e){
                std::cerr << e.what() << std::endl;
                emergency_leave();
            }
            try{
                WindowLoop(window_main);
            }
            catch(std::exception& e){
                std::cerr << e.what() << std::endl;
                emergency_leave();
            }
            return window_main;
        }

        void cleanup(Main_Window* window_main){
            ImGui_ImplOpenGL3_Shutdown();
            ImGui_ImplGlfw_Shutdown();
            ImGui::DestroyContext();
            glfwDestroyWindow(window_main->window);
            glfwTerminate();
        }
    }
}
