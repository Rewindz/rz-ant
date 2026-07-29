#pragma once
#include <cstdint>
#include <format>
#include <vector>
#include <random>
#include <algorithm>
#include <span>
#include <string>
#include <unordered_map>
#include <format>

enum class DIRECTION
{
    UP = 0,
    RIGHT,
    DOWN,
    LEFT
};

struct Ant
{
    int x = 0;
    int y = 0;
    DIRECTION curDirection = DIRECTION::UP;

    Ant() = default;
    Ant(int _x, int _y, DIRECTION _dir = DIRECTION::UP)
        : x(_x), y(_y), curDirection(_dir) {}

    inline void turnRight()
    {
        curDirection = static_cast<DIRECTION>((static_cast<int>(curDirection) + 1) % 4);
    }

    inline void turnLeft()
    {
        curDirection = static_cast<DIRECTION>((static_cast<int>(curDirection) + 3) % 4);
    }

    inline void move(int grid_w, int grid_h)
    {
        switch(curDirection)
        {
            case DIRECTION::UP:    y--; break;
            case DIRECTION::RIGHT: x++; break;
            case DIRECTION::DOWN:  y++; break;
            case DIRECTION::LEFT:  x--; break;
        }

        if(x < 0) x = grid_w - 1;
        else if(x >= grid_w) x = 0;

        if(y < 0) y = grid_h - 1;
        else if(y >= grid_h) y = 0;
    }

};

enum class CELL_DIR : std::uint8_t
{
    RIGHT = 1,
    LEFT = 3
};

using Color = std::uint32_t;

inline Color GetColorFromRGB(std::uint8_t R, std::uint8_t G, std::uint8_t B)
{
    Color res = 0;
    res ^= R << 16;
    res ^= G << 8;
    res ^= B;
    return res;
}

constexpr Color COLOR_WHITE  = 0x00FFFFFF;
constexpr Color COLOR_BLACK  = 0x00000000;
constexpr Color COLOR_RED    = 0x00FF0000;
constexpr Color COLOR_GREEN  = 0x0000FF00;
constexpr Color COLOR_BLUE   = 0x000000FF;

inline Color GetRandomUniqueColor(std::span<Color> existingColors)
{
    static std::mt19937 rng(std::random_device{}());
    static std::uniform_int_distribution<Color> dist(0x000000, 0xFFFFFF);
    Color newColor;
    while(true) {
        newColor = dist(rng);
        auto it = std::find(existingColors.begin(), existingColors.end(), newColor);
        if(it == existingColors.end())
            return newColor;
    }
}

inline void FillRandomColors(std::span<Color> colors)
{
    for(std::size_t i = 0; i < colors.size(); i++)
        colors[i] = GetRandomUniqueColor(colors.subspan(0, i));
}

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
