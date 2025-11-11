#pragma once

#include <iostream>
#include <string>

enum Ast
{
    Add,
    Sub,
    Next,
    Prev,
    LoopBegin,
    Print,
    Input,
};

struct Instruction
{
    Ast ast;

    union
    {
        int repeat;
        int loopLen;
    };

    void print(Instruction self)
    {
        std::string astStr = " ";
        switch (self.ast)
        {
            case Add:
            {
                astStr = "Add";
                break;
            }
            case Sub:
            {
                astStr = "Sub";
                break;
            }
            case Next:
            {
                astStr = "Next";
                break;
            }
            case Prev:
            {
                astStr = "Prev";
                break;
            }
            case LoopBegin:
            {
                astStr = "Loop";
                break;
            }
            case Print:
            {
                astStr = "Print";
                break;
            }
            case Input:
            {
                astStr = "Input";
                break;
            }
        };
        std::cout << astStr << " with inner " << self.repeat << " val\n";
    };
};

struct Loop
{
    int pos;
    int len;
};
