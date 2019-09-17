#include <iostream>
#include "Board.h"
#include "TripleTriad.h"

int main(int argc, char* argv[]) {
    using namespace TripleTriad;
    std::stack<Board> states;
    auto main = Board({Same, SameWall, Plus});
    auto c = Card::Factory("Edea", Blue);
    for (int i = 0; i < 9; ++i)
        std::cout<<"Pos: "<<i<<','<<main.check(c, i)<<std::endl;
//    main.play(Card::Factory("Ruby Dragon", Red), 1);

    return 0;
}