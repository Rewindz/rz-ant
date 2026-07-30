#pragma once

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
