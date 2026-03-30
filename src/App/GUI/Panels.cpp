#include "App/GUI/Panels.h"
#include "App/Utils/SystemStats.h"
#include "App/Utils/Formatters.h"

#include "imgui.h"
#include "../implot/implot.h"

#include <vector>

namespace APP {
    namespace GUI {
        void GetDynamicWindowLayout(int window_index, ImVec2& pos, ImVec2& size) {
            ImGuiViewport* viewport = ImGui::GetMainViewport();
            float avail_width = viewport->WorkSize.x;
            float avail_height = viewport->WorkSize.y;
            
            float window_width = avail_width / 2.0f;
            float window_height = avail_height / 2.0f;
            
            switch(window_index) {
                case 0:
                    pos = ImVec2(viewport->WorkPos.x, viewport->WorkPos.y);
                    break;
                case 1: 
                    pos = ImVec2(viewport->WorkPos.x + window_width, viewport->WorkPos.y);
                    break;
                case 2:
                    pos = ImVec2(viewport->WorkPos.x, viewport->WorkPos.y + window_height);
                    break;
                case 3: 
                    pos = ImVec2(viewport->WorkPos.x + window_width, viewport->WorkPos.y + window_height);
                    break;
                default:
                    pos = ImVec2(viewport->WorkPos.x, viewport->WorkPos.y);
            }
            
            size = ImVec2(window_width, window_height);
        }

        void StatPanel()
        {
            auto P = Utils::GetSystemData();

            ImGuiWindowFlags window_flags = 
                ImGuiWindowFlags_NoMove |        
                ImGuiWindowFlags_NoCollapse;

            ImVec2 pos, size;
            GetDynamicWindowLayout(0, pos, size);
            
            ImGui::SetNextWindowPos(pos, ImGuiCond_Always); 
            ImGui::SetNextWindowSize(size, ImGuiCond_Always);

            ImGui::Begin("Statistics", nullptr, window_flags);

            ImGui::Text("Free RAM: %s", Utils::format_bytes(P->FreeRam).c_str());
            ImGui::Text("Total RAM: %s", Utils::format_bytes(P->TotalRam).c_str());
            ImGui::Text("Uptime: %s", Utils::format_time(P->uptime).c_str());

            ImGui::Text("Max Childs per Process: %lu", P->ChildMax);
            ImGui::Text("Clock ticks: %lu", P->Clockticks);
            ImGui::Text("POSIX Version: %lu", P->POSIX_VERSION);
            ImGui::Text("Active Processors: %d", P->Processors);

            ImGui::Text("OS Name: %s", P->OSName.c_str());
            ImGui::Text("OS Version: %s", P->OSVersion.c_str());
            ImGui::Text("Architecture: %s", P->Architecture.c_str());
            ImGui::Text("Distro: %s", P->Distro.c_str());

            ImGui::Text("CPU Model: %s", P->CPUModel.c_str());

            ImGui::Text("Current User: %s", P->Username.c_str());
            ImGui::Text("Hostname: %s", P->Hostname.c_str());

            if (!P->NetworkInterfaces.empty()) {
                ImGui::Separator();
                ImGui::Text("Network Interfaces:");
                for (const auto& iface : P->NetworkInterfaces) {
                    ImGui::Text("%s: %s", iface.Name.c_str(), iface.IP.c_str());
                }
            }

            ImGui::End();
        }

        void RAMPlotPanel()
        {
            auto P = Utils::GetSystemData();
            auto& ram_history = Utils::RAMHistory::getInstance();

            ImGuiWindowFlags window_flags = 
                ImGuiWindowFlags_NoMove |
                ImGuiWindowFlags_NoCollapse;     

            ImVec2 pos, size;
            GetDynamicWindowLayout(1, pos, size);
            
            ImGui::SetNextWindowPos(pos, ImGuiCond_Always); 
            ImGui::SetNextWindowSize(size, ImGuiCond_Always);

            ImGui::Begin("RAM Monitor", nullptr, window_flags);

            float used_ram_mb = (P->TotalRam - P->FreeRam) / (1024.0f * 1024.0f);
            float total_ram_mb = P->TotalRam / (1024.0f * 1024.0f);
            float free_ram_mb = P->FreeRam / (1024.0f * 1024.0f);
            float usage_percent = (used_ram_mb / total_ram_mb) * 100.0f;

            ImGui::Text("Total RAM: %.2f MB", total_ram_mb);
            ImGui::Text("Used RAM: %.2f MB (%.1f%%)", used_ram_mb, usage_percent);
            ImGui::Text("Free RAM: %.2f MB", free_ram_mb);

            ImGui::Separator();

            static int plot_type = 0;
            ImGui::Text("Plot Type:");
            ImGui::RadioButton("Line Chart", &plot_type, 0); ImGui::SameLine();
            ImGui::RadioButton("Pie Chart", &plot_type, 1); ImGui::SameLine();
            ImGui::RadioButton("Bar Chart", &plot_type, 2);

            ImGui::Separator();

            const auto& free_history = ram_history.getFreeRamHistory();
            const auto& used_history = ram_history.getUsedRamHistory();

            if (!free_history.empty()) {
                std::vector<float> used_vec(used_history.begin(), used_history.end());
                std::vector<float> free_vec(free_history.begin(), free_history.end());

                float plot_height = ImGui::GetContentRegionAvail().y - 10;

                if (plot_type == 0) {
                    if (ImPlot::BeginPlot("RAM Usage Over Time", ImVec2(-1, plot_height))) {
                        ImPlot::SetupAxes("Time (samples)", "RAM (MB)");
                        ImPlot::SetupAxisLimits(ImAxis_X1, 0, ram_history.getMaxSamples(), ImGuiCond_Always);
                        ImPlot::SetupAxisLimits(ImAxis_Y1, 0, total_ram_mb * 1.1f, ImGuiCond_Once);

                        ImPlot::PlotLine("Used RAM", used_vec.data(), (int)used_vec.size());
                        ImPlot::PlotLine("Free RAM", free_vec.data(), (int)free_vec.size());

                        ImPlot::EndPlot();
                    }
                } else if (plot_type == 1) {
                    float ram_data[] = {used_ram_mb, free_ram_mb};
                    const char* ram_labels[] = {"Used", "Free"};
                    
                    if (ImPlot::BeginPlot("Current RAM Usage", ImVec2(-1, plot_height), ImPlotFlags_Equal)) {
                        ImPlot::PlotPieChart(ram_labels, ram_data, 2, 0.5, 0.5, 0.4);
                        ImPlot::EndPlot();
                    }
                } else if (plot_type == 2) {
                    float bar_data[] = {used_ram_mb, free_ram_mb};
                    
                    if (ImPlot::BeginPlot("RAM Distribution", ImVec2(-1, plot_height))) {
                        ImPlot::SetupAxes("Category", "RAM (MB)");
                        ImPlot::SetupAxisLimits(ImAxis_Y1, 0, total_ram_mb * 1.1f, ImGuiCond_Once);
                        ImPlot::PlotBars("RAM", bar_data, 2);
                        ImPlot::EndPlot();
                    }
                }
            }

            ImGui::End();
        }
    }
}