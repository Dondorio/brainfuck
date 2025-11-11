#include <cstdint>
#include <cstdio>
#include <vector>

#include "eval.hpp"
#include "globabs.hpp"
#include "types.hpp"

void eval(const std::vector<Instruction>& instructions, uint8_t data[],
          uint32_t& cellPointer, const uint32_t& sliceBegin,
          const uint32_t& sliceEnd)
{
    for (uint32_t i = sliceBegin; i < sliceEnd; i++)
    {
        switch (instructions[i].ast)
        {
            case Add:
            {
                data[cellPointer] += instructions[i].repeat;
                break;
            }
            case Sub:
            {
                data[cellPointer] -= instructions[i].repeat;
                break;
            }
            case Next:
            {
                cellPointer += instructions[i].repeat;

                if (cellPointer > DATA_SIZE)
                    throw 0;

                break;
            }
            case Prev:
            {
                cellPointer -= instructions[i].repeat;

                if (cellPointer < 0)
                    throw 0;

                break;
            }
            case LoopBegin:
            {
                int looplen = instructions[i].loopLen;

                while (data[cellPointer] != 0)
                {
                    eval(instructions, data, cellPointer, i + 1, i + looplen);
                }

                // Skip over items inside loop
                // -1 because the for loop already adds 1
                i += looplen - 1;

                break;
            }
            case Print:
            {
                for (int p = 0; p < instructions[i].repeat; p++)
                {
                    putchar(data[cellPointer]);
                }

                break;
            }
            case Input:
            {
                data[cellPointer] = getchar();
                break;
            }
        }
    }
}
