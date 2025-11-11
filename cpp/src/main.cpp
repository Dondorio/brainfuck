#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "eval.hpp"
#include "globabs.hpp"
#include "reader.hpp"
#include "types.hpp"

struct Opts
{
    bool repl{false};
    bool verbose{false};
    int data_size{30000}; // TODO
    std::string file{""};
};

// TODO help text
void print_help() { printf("help\n"); }

int main(int argc, char* argv[])
{
    Opts opts;

    for (int i = 1; i < argc; i++)
    {
        char* arg = argv[i];

        // eww
        if (strcmp(arg, "--help") == 0)
        {
            print_help();
            return 0;
        }
        if (strcmp(arg, "--repl") == 0)
        {
            opts.repl = true;
            continue;
        }
        if (strcmp(arg, "--verbose") == 0 || strcmp(arg, "-v") == 0)
        {
            opts.verbose = true;
            continue;
        }
        if (strcmp(arg, "-e") == 0)
        {
            if (argc > i + 1)
            {
                i++;
                opts.file = argv[i];
                continue;
            }

            printf("Expected file after -e, found none\n");
            return 0;
        }

        // ./brainfuck filename.bf
        if (i == 1)
        {
            opts.file = arg;
            continue;
        }

        printf("Invalid argument: %s\n", arg);
        return 0;
    }

    std::string chars;

    if (opts.repl)
    {
        std::getline(std::cin, chars);
    }
    else
    {
        if (opts.file == "")
        {
            printf("Expected file, foud none");
            return 0;
        }

        // TODO check if file exists
        std::ifstream f;
        f.open(opts.file);

        std::stringstream strStream;
        strStream << f.rdbuf();
        chars = strStream.str();
    }

    std::vector<Instruction> instructions;

    try
    {
        instructions = reader::readStr(chars);
    }
    catch (reader::Error err)
    {
        printf("Reader error: ");
        switch (err)
        {
            case reader::BracketEof:
            {
                printf("expected ']', found EOF\n");
                break;
            }
            case reader::NoOpenBracket:
            {
                printf("found ']' without opening '['\n");
                break;
            }
        }
        return 0;
    }

    uint8_t data[DATA_SIZE] = {};
    uint32_t cellPointer = 0;

    try
    {
        eval(instructions, data, cellPointer, 0, instructions.size());
    }
    catch (...)
    {
        printf("Evaluation error: out of bounds\n");
    }

    if (opts.verbose)
    {
        printf("----- data [1..30] -----\n");

        for (int i = 0; i < 30; i++)
        {
            printf("%d; ", data[i]);
        }
    }

    return 0;
}
