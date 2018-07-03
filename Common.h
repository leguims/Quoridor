#pragma once

#include <string>

using PlayerName = std::string;

enum class Color { none, red, blue, white, black };
inline std::ostream& operator<<(std::ostream& out, const Color& color)
{
    switch (color)
    {
    case Color::black:
        out << 'B';
        break;
    case Color::blue:
        out << 'U';
        break;
    case Color::red:
        out << 'R';
        break;
    case Color::white:
        out << 'W';
        break;
    }
    return out;
}

