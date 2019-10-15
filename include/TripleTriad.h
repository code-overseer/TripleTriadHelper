#ifndef TRIPLETRIAD_TRIPLETRIAD_H
#define TRIPLETRIAD_TRIPLETRIAD_H

#include "Utils.h"
namespace TripleTriad {
    cxxopts::Options option_parser();
    std::vector<Card> player_cards(std::string const& names, Team team);
    struct hint_t {
        std::string name;
        int position = -1;
        float score = 0;
    };

    struct state_t {
        std::string name;
        int position = -1;
        float score = 0;
        Board board;
        std::vector<Card> const* player;
        std::vector<Card> const* enemy;
        bool* player_used;
        bool* enemy_used;
    };
    void open_game(cxxopts::ParseResult const &result);
    void close_game(cxxopts::ParseResult const &result);
    std::string get_hint(Board const &main, std::vector<Card> const &player, std::vector<Card> const &enemy);

    void test_open();
}


#endif //TRIPLETRIAD_TRIPLETRIAD_H
