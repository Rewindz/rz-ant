#include "Menu.hpp"

#include <string>
#include <algorithm>

#include <imgui.h>
#include <misc/cpp/imgui_stdlib.h>
#include <GLFW/glfw3.h>

bool Menu::validateRulesText()
{
    if(values.rules.empty())
        return false;

    return std::ranges::all_of(values.rules, [](char c){
        return c == 'L' || c == 'R';
    });
}

void Menu::Render(bool visible)
{
    if(!visible) return;

    bool recreateGrid, rulesDirty;
    recreateGrid = rulesDirty = false;

    int tW = static_cast<int>(grid.w);
    int tH = static_cast<int>(grid.h);

    ImGui::SetNextWindowSize(ImVec2(0.0f, 0.0f), ImGuiCond_FirstUseEver);

    ImGui::Begin("Sim Controls");
        ImGui::Text("Right-click off of this menu to toggle it.");
        ImGui::InputInt("Steps per frame", &values.simSteps, 1, 10);
        if(ImGui::InputInt("Grid Width", &tW, 1, 10)) {
            grid.w = tW;
            recreateGrid = true;
        }
        if(ImGui::InputInt("Grid Height", &tH, 1, 10)) {
            grid.h = tH;
            recreateGrid = true;
        }
        if(ImGui::InputText("Rules [L|R]", &values.rules, ImGuiInputTextFlags_EnterReturnsTrue)) {
            rulesDirty = true;
        }
        if(ImGui::Button("Restart")) {
            recreateGrid = true;
        }
        ImGui::SameLine();
        if(ImGui::Button(values.paused ? "Play" : "Pause")) {
            values.paused = !values.paused;
        }
        ImGui::Separator();
        if(ImGui::Checkbox("VSync", &values.vsync)) {
            glfwSwapInterval(values.vsync);
        }
        ImGui::SameLine();
        ImGui::Text("FPS: %.1f", ImGui::GetIO().Framerate);
        ImGui::Separator();
    ImGui::End();

    if(rulesDirty) {
        if(validateRulesText())
            lastValidRulesText = values.rules;
        else {
            values.rules = lastValidRulesText;
            return;
        }
    }

    if(recreateGrid || rulesDirty)
        grid = Grid(grid.w, grid.h, CreateRulesFromString(values.rules));

}
