#ifndef TRIPLETRIAD_TRIPLETRIAD_H
#define TRIPLETRIAD_TRIPLETRIAD_H

#include "Utils.h"
namespace TripleTriad {
    class Board;
    class Card;
    cxxopts::Options option_parser();
    std::vector<std::string> player_cards(std::string const &names);
    void open_game(cxxopts::ParseResult const &result);
    void close_game(cxxopts::ParseResult const &result);
    std::string get_hint(Board const &main, std::vector<Card> const &player, std::vector<Card> const &enemy);

    void test_open();
    void test_hint();
}


#endif //TRIPLETRIAD_TRIPLETRIAD_H
