#include "debug_screen.h"
#include <imgui_impl_sdl.h>
#include <iostream>

DebugScreen::DebugScreen(Chip8 &chip8): chip8(chip8) {

}

void DebugScreen::update() {
    // Draw Menubar
    ImGui::BeginMainMenuBar();
    if (ImGui::BeginMenu("File")) {
        if (ImGui::MenuItem("Open...")) {
        }
        if (ImGui::MenuItem("Reset ROM")) {
        }
        ImGui::EndMenu();
    }
    if(ImGui::BeginMenu("Extra")) {
        if (ImGui::MenuItem("Demo")) {
            if (ImGui::MenuItem("Demo")) {
                showDemo = !showDemo;
                std::cout << "Clicked" << std::endl;
            }
        }
        ImGui::EndMenu();
    }
    ImGui::EndMainMenuBar();

    if (ImGui::Button("Button")) {
        counter++;
        showDemo = !showDemo;
    }
    
    ImGui::SameLine();
    ImGui::Text("counter = %d", counter);
    ImGui::Text("%s", showDemo ? "true" : "false");

    if (showDemo) {
        ImGui::ShowDemoWindow(&showDemo);
    }
}
