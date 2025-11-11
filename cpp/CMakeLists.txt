cmake_minimum_required(VERSION 3.16)
project(brainfuck LANGUAGES CXX)

# set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
# set(CMAKE_CXX_STANDARD 23)
# set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB_RECURSE SOURCES
    src/*.cpp
    src/*.hpp
)

add_executable(${PROJECT_NAME} ${SOURCES})
