cmake_minimum_required(VERSION 3.14)
project(TripleTriad)

set(CMAKE_CXX_STANDARD 14)
INCLUDE_DIRECTORIES(include)

add_executable(TripleTriad
        main.cpp
        include/TripleTriad.h
        include/Card.h
        include/Board.h
        src/Board.cpp
        src/Card.cpp include/Position.h src/Position.cpp)