cmake_minimum_required(VERSION 3.14)
project(TripleTriad)

set(CMAKE_CXX_STANDARD 14)
INCLUDE_DIRECTORIES(include)
find_package (Threads)
add_executable(TripleTriad
        main.cpp
        include/TripleTriad.h
        include/Card.h
        include/Board.h
        include/Position.h
        include/utils.h
        src/Board.cpp
        src/Card.cpp
        src/Position.cpp
        src/TripleTriad.cpp)
target_link_libraries (TripleTriad ${CMAKE_THREAD_LIBS_INIT})

add_definitions(-DCARD_DATA="${CMAKE_SOURCE_DIR}/Cards.csv")