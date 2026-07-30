#pragma once
#include <string>

#include <imgui.h>

#include "Ant.hpp"
#include "Grid.hpp"

struct MenuValues
{
    int simSteps = 10;
    std::string rules = "LRLRRRLR";
    bool vsync = true;
    bool paused = true;
};

class Menu
{
public:
    Menu(int defaultWidth, int defaultHeight)
        : values(), grid(defaultWidth, defaultHeight, CreateRulesFromString(values.rules))
    {
        lastValidRulesText = values.rules;
    }

    void Render(bool visible);

    inline Grid& GetGrid() { return grid; }

    MenuValues values;

private:

    Grid grid;
    std::string lastValidRulesText;

    bool validateRulesText();

};
