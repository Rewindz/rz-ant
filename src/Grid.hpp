#pragma once

#include <vector>
#include <cstdint>
#include <format>

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

struct Rule
{
    bool turnRight;
    std::uint8_t nextState;
    Color nextColor;
};

struct Grid
{
    Grid(std::size_t _w, std::size_t _h, std::span<const CELL_DIR> _rules)
        : w(_w), h(_h), ant(_w / 2, _h / 2), states(_w * _h, 0), pixels(_w * _h)
    {
        std::vector<Color> colorSeq(_rules.size());
        FillRandomColors(colorSeq);

        rules.resize(_rules.size());
        for(std::size_t i = 0; i < _rules.size(); i++) {
            std::uint8_t nextIdx = (i + 1) % _rules.size();
            rules[i].turnRight = (_rules[i] == CELL_DIR::RIGHT);
            rules[i].nextState = nextIdx;
            rules[i].nextColor = colorSeq[nextIdx];
        }

        for(auto& p : pixels) {
            p = colorSeq[0];
        }
    }

    inline void simulate()
    {
        int curIdx = ant.y * w + ant.x;
        std::uint8_t curState = states[curIdx];
        const auto& rule = rules[curState];

        if(rule.turnRight)
            ant.turnRight();
        else
            ant.turnLeft();

        states[curIdx] = rule.nextState;
        pixels[curIdx] = rule.nextColor;
        ant.move(w, h);
    }

    inline const Color* getData() const { return pixels.data(); }

    Ant ant;
    std::size_t w, h;

private:
    std::vector<std::uint8_t> states;
    std::vector<Color> pixels;
    std::vector<Rule> rules;

};
