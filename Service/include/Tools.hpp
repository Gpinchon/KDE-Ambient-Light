#pragma once

#include <string>

std::string exec(const std::string &a_Cmd);

float lerp(float a, float b, float t)
{
    return a + t * (b - a);
}