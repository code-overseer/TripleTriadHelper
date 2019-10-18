#ifndef TRIPLETRIAD_TRIPLETRIAD_H
#define TRIPLETRIAD_TRIPLETRIAD_H

#include "Utils.h"

namespace TripleTriad {
    class Board;
    cxxopts::Options option_parser();
    std::vector<std::string> player_cards(std::string const &names);
    void open_game(cxxopts::ParseResult const &result);
    void close_game(cxxopts::ParseResult const &result);
    std::string hint(Board const &main, std::vector<std::string> const &player,
                     std::vector<std::string> const &enemy);
    void test_open();
    void test_hint();
}


#endif //TRIPLETRIAD_TRIPLETRIAD_H
