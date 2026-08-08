#pragma once

#include <random>
#include <cstdint>
#include <span>
#include <algorithm>
#include <array>

using Color = std::uint32_t;

inline Color ColorFromRGB(std::uint8_t R, std::uint8_t G, std::uint8_t B)
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

inline Color ColorFromFloats(std::array<float, 3>& _colors) {
    return ColorFromRGB(
        static_cast<uint8_t>(_colors[0] * 255.f),
        static_cast<uint8_t>(_colors[1] * 255.f),
        static_cast<uint8_t>(_colors[2] * 255.f)
    );
}

inline std::array<float, 3> FloatsFromColor(Color& _color) {
    std::array<float, 3> res;
    res[0] = static_cast<float>((_color >> 16) & 0xFF) / 255.f;
    res[1] = static_cast<float>((_color >> 8) & 0xFF) / 255.f;
    res[2] = static_cast<float>(_color & 0xFF) / 255.f;
    return res;
}
