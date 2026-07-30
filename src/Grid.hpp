#pragma once

#include <vector>
#include <cstdint>
#include <format>
#include <unordered_map>

#include "Color.hpp"
#include "Ant.hpp"

enum class CELL_DIR : std::uint8_t
{
    RIGHT = 1,
    LEFT = 3
};

struct Cell
{
    CELL_DIR dir = CELL_DIR::RIGHT;
    Color *color = nullptr;
};

inline std::vector<CELL_DIR> CreateRulesFromString(const std::string& rules)
{
    std::vector<CELL_DIR> res;
    res.reserve(rules.length());
    for(const auto& a : rules) {
        if(a == 'L')
            res.push_back(CELL_DIR::LEFT);
        else if (a == 'R')
            res.push_back(CELL_DIR::RIGHT);
        else
            throw std::format_error(std::format("Rules string has a bad format! {}", rules));
    }
    return res;
}

struct RuleState
{
    CELL_DIR turnDir;
    Color nextColor;
};

struct Grid
{
    Grid(std::size_t _w, std::size_t _h, std::span<const CELL_DIR> rules)
        : cells(_w * _h), cellColors(_w * _h), w(_w), h(_h), ant(_w / 2, _h / 2)
    {
        std::vector<Color> sequenceColors(rules.size());
        FillRandomColors(sequenceColors);

        for(std::size_t i = 0; i < rules.size(); i++) {
            Color nextColor = sequenceColors[(i + 1) % rules.size()];
            ruleMap[sequenceColors[i]] = { rules[i], nextColor };
        }

        Color defaultColor = sequenceColors[0];
        for(std::size_t i = 0; i < _w * _h; i++) {
            cellColors[i] = defaultColor;
            cells[i].color = cellColors.data() + i;
        }
    }

    inline void simulate()
    {
        int curIdx = ant.y * w + ant.x;

        Color curColor = *cells[curIdx].color;
        const RuleState& state = ruleMap.at(curColor);
        if(state.turnDir == CELL_DIR::RIGHT)
            ant.turnRight();
        else
            ant.turnLeft();
        *cells[curIdx].color = state.nextColor;
        ant.move(w, h);
    }

    inline Color* getData() { return cellColors.data(); }

    std::vector<Cell> cells;

    Ant ant;
    std::size_t w, h;

private:
    std::vector<Color> cellColors;
    std::unordered_map<Color, RuleState> ruleMap;
    Color* nextColor = nullptr;
};
