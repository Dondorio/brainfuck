#pragma once

#include <string>
#include <vector>

#include "types.hpp"

namespace reader
{
enum Error
{
    BracketEof,
    NoOpenBracket,
};

std::vector<Instruction> readStr(std::string str);
} // namespace reader
