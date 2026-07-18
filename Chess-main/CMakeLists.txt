cmake_minimum_required(VERSION 3.10.0)
project(Chess VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(SFML COMPONENTS Graphics Window System REQUIRED)

if(NOT SFML_FOUND)
    message(FATAL_ERROR "SFML not found! Please install SFML")
endif()

add_executable(Chess-main
    main.cpp
    Menu.cpp
    ChessBoard.cpp
    board.cpp
    ai.cpp
    fen.cpp
    PieceRenderer.cpp
    figures.cpp
)

target_link_libraries(Chess-main PRIVATE
    sfml-graphics
    sfml-window
    sfml-system
)


file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/font.ttf 
     DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/assets)
    file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/assets 
         DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
endif()