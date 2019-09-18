#include "Board.h"
#include "TripleTriad.h"

int main(int argc, char* argv[]) {
    using namespace TripleTriad;
    auto main = Board({Same, SameWall, Plus});
    auto c = Card::Factory("Edea", Blue);
    main.hint({c});

    return 0;
}