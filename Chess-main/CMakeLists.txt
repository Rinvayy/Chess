cmake_minimum_required(VERSION 3.10.0)
project(chessCode VERSION 0.1.0 LANGUAGES C CXX)

add_executable(Chess-main
    main.cpp
    board.cpp
    figures.cpp
    ai.cpp
)

