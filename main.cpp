#include <iostream>
#include "Board.h"
#include "TripleTriad.h"

int main(int argc, char* argv[]) {
    using namespace TripleTriad;
    std::stack<Board> states;
    auto main = new Board({Same, SameWall, Plus});
    states.emplace(*main);

    return 0;
}