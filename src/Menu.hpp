#pragma once
#include <string>
#include <functional>

#include <imgui.h>

#include "Ant.hpp"
#include "Grid.hpp"

struct MenuValues
{
    int simSteps = 10;
    std::string rules = "LRLRRRLR";
    bool vsync = true;
    bool paused = true;
    bool step = false;
};

struct MenuCallbacks
{
    std::function<void()> panRestore = nullptr;
    std::function<void()> zoomRestore = nullptr;
};


class Menu
{
public:
    Menu(int defaultWidth, int defaultHeight, MenuCallbacks& cbs)
        : values(), grid(defaultWidth, defaultHeight, CreateRulesFromString(values.rules)), callbacks(cbs)
    {
        lastValidRulesText = values.rules;
    }

    void Render(bool visible);

    inline Grid& GetGrid() { return grid; }

    MenuValues values;

private:

    MenuCallbacks& callbacks;
    Grid grid;
    std::string lastValidRulesText;

    bool validateRulesText();

};
