#pragma once
#include <cstdint>

enum class DIRECTION : std::uint8_t
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
        curDirection = static_cast<DIRECTION>((static_cast<int>(curDirection) + 1) & 3);
    }

    inline void turnLeft()
    {
        curDirection = static_cast<DIRECTION>((static_cast<int>(curDirection) + 3) & 3);
    }

    inline void move(int grid_w, int grid_h)
    {
        static constexpr int dx[4] = {  0,  1,  0, -1 };
        static constexpr int dy[4] = { -1,  0,  1,  0 };

        x += dx[static_cast<uint8_t>(curDirection)];
        y += dy[static_cast<uint8_t>(curDirection)];

        if(x < 0) [[unlikely]] x = grid_w - 1;
        else if(x >= grid_w) [[unlikely]]  x = 0;

        if(y < 0) [[unlikely]] y = grid_h - 1;
        else if(y >= grid_h) [[unlikely]] y = 0;
    }

};
