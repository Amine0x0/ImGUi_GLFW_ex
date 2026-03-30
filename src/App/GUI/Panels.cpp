#include "App/GUI/Panels.h"
#include "App/Utils/SystemStats.h"
#include "App/Utils/Formatters.h"

#include "imgui.h"

namespace APP {
    namespace GUI {
        void StatPanel(void)
        {
            std::unique_ptr<Utils::Process> P = Utils::GetData();

            static bool ram_analytics = false;
            static bool General_stats = false;
            static int dsize = 0;

            ImGui::Begin("Statistics");

            ImGui::Text("This is a small Statistics Panel for this current instance");

            ImGui::Checkbox("Show RAM stats", &ram_analytics);
            ImGui::Checkbox("Show General System Stats", &General_stats);

            if (ram_analytics)
                std::string ram = Utils::format_bytes(P->FreeRam);

            if (General_stats)
                std::string up = Utils::format_time(P->uptime);

            if (ImGui::TreeNode("SHOW FULL"))
            {
                if (ram_analytics)
                {
                    if (ImGui::TreeNode("RAM"))
                    {
                        ImGui::Text("Free: %s", Utils::format_bytes(P->FreeRam).c_str());
                        ImGui::TreePop();
                    }
                }

                if (General_stats)
                {
                    if (ImGui::TreeNode("Uptime"))
                    {
                        ImGui::Text("%s", Utils::format_time(P->uptime).c_str());
                        ImGui::TreePop();
                    }
                }

                ImGui::TreePop();
            }

            if (ImGui::Button("Press here to increase Dsize lul"))
                dsize++;

            ImGui::SameLine();
            ImGui::Text("dick size = %d", dsize);

            ImGui::End();
        }
    }
}
