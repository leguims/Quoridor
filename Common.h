#pragma once

#include <string>

using PlayerName = std::string;

enum class Color { none, red, green, blue, white, black };
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
    case Color::green:
        out << 'G';
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

inline std::string color(Color c)
{
    switch (c)
    {
    case Color::black:
        return "black";
    case Color::blue:
        return "blue";
    case Color::green:
        return "green";
    case Color::red:
        return "red";
    case Color::white:
        return "white";
    }
    return "";
}
