#include "TripleTriad.h"

int main(int argc, char* argv[]) {
    using namespace TripleTriad;
    auto options = option_parser();
    auto result = options.parse(argc, argv);

    bool isOpen = result["red"].count();
    if (isOpen) {
        open_game(result);
    } else {
        close_game(result);
    }

    return 0;
}