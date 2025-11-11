#include <cstdio>
#include <string>
#include <vector>

#include "reader.hpp"
#include "types.hpp"

namespace reader
{
std::vector<Instruction> readStr(std::string str)
{
    std::string chars = str;
    chars.push_back(0);

    std::vector<Instruction> instructions;

    char lastChar = 0;
    int rep = 0;

    std::vector<Loop> pendingLoops;

    for (int i = 0; i <= chars.length(); i++)
    {
        char currentChar = chars[i];

        if (i == 0)
        {
            lastChar = currentChar;
            continue;
        }

        if (currentChar == lastChar && i < chars.length() - 1 &&
            currentChar != '[' && currentChar != ']')
        {
            rep++;
            lastChar = currentChar;
            continue;
        }

        rep++;
        Instruction tmp;

        switch (lastChar)
        {
            case '+':
            {
                tmp.ast = Ast::Add;
                break;
            }
            case '-':
            {
                tmp.ast = Ast::Sub;
                break;
            }
            case '<':
            {
                tmp.ast = Ast::Prev;
                break;
            }
            case '>':
            {
                tmp.ast = Ast::Next;
                break;
            }
            case '.':
            {
                tmp.ast = Ast::Print;
                break;
            }
            case ',':
            {
                tmp.ast = Ast::Input;
                break;
            }
            case '[':
            {
                tmp.ast = Ast::LoopBegin;
                instructions.push_back(tmp);

                pendingLoops.push_back(
                    {static_cast<int>(instructions.size()), 0});

                rep = 0;
                lastChar = currentChar;

                for (int i = 0; i < pendingLoops.size(); i++)
                {
                    pendingLoops[i].len++;
                }
                continue;
            }
            case ']':
            {
                if (pendingLoops.empty())
                    throw Error::NoOpenBracket;

                Loop l = pendingLoops.back();
                pendingLoops.pop_back();

                instructions[l.pos - 1].loopLen = l.len;

                rep = 0;
                lastChar = currentChar;

                continue;
            }
            default:
            {
                rep = 0;
                lastChar = currentChar;
                continue;
            }
        }

        for (int i = 0; i < pendingLoops.size(); i++)
        {
            pendingLoops[i].len++;
        }

        tmp.repeat = rep;

        lastChar = currentChar;
        rep = 0;
        instructions.push_back(tmp);
    }

    if (!pendingLoops.empty())
        throw Error::BracketEof;

    return instructions;
}
} // namespace reader
