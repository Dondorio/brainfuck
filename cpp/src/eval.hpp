#pragma once

#include <cstdint>
#include <vector>

#include "types.hpp"

void eval(const std::vector<Instruction>& instructions, uint8_t data[],
          uint32_t& cellPointer, const uint32_t& sliceBegin,
          const uint32_t& sliceEnd);
