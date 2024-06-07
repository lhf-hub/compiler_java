cmake_minimum_required(VERSION 3.28)
project(LL1)

set(CMAKE_CXX_STANDARD 17)

add_executable(LL1 main.cpp
        lexer.cpp
        lexer.h
        syntax.cpp
        syntax.h
        grammar.h
        predict.h
)
