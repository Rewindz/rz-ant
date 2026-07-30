#pragma once

#include <random>
#include <cstdint>
#include <span>
#include <algorithm>

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
