#include "Board.h"
#include "TripleTriad.h"

int main(int argc, char* argv[]) {
    using namespace TripleTriad;
    auto options = option_parser();
    auto result = options.parse(argc, argv);
//    test_open();
    test_hint();
    return 0;
    std::vector<Card> blue = player_cards(result["blue"].as<std::string>());
    bool isOpen = result["red"].count();
    if (isOpen) {
        open_game(result);
    } else {
        close_game(result);
    }

    return 0;
}