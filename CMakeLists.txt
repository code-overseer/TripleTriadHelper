cmake_minimum_required(VERSION 3.14)
project(TripleTriad)
set(CMAKE_CXX_STANDARD 14)

INCLUDE_DIRECTORIES(include)
find_package(Curses REQUIRED)
add_executable(TripleTriad
        main.cpp
        include/TripleTriad.h
        include/Card.h
        include/Board.h
        include/Position.h
        include/GUI.h
        include/Utils.h
        include/Node.h
        include/Branch.h
        src/Board.cpp
        src/Card.cpp
        src/Position.cpp
        src/TripleTriad.cpp
        src/GUI.cpp
        src/Node.cpp
        src/Branch.cpp)
target_link_libraries (TripleTriad ${CMAKE_THREAD_LIBS_INIT})
target_link_libraries(TripleTriad ${CURSES_LIBRARIES})

add_definitions(-DCARD_DATA="${CMAKE_SOURCE_DIR}/Cards.csv")