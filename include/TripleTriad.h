#ifndef TRIPLETRIAD_TRIPLETRIAD_H
#define TRIPLETRIAD_TRIPLETRIAD_H

#include "Utils.h"

namespace TripleTriad {
    class Node;
    class Board;
    class Card;
    struct score_t {
        int pos;
        int card;
        float val;
        static bool compare(const score_t& left, const score_t& right) { return left.val > right.val; };
        score_t(int pos, int uid, float val) : pos(pos), card(uid), val(val) {}
    };
    cxxopts::Options option_parser();
    std::vector<std::string> player_cards(std::string const &names);
    void open_game(cxxopts::ParseResult const &result);
    void close_game(cxxopts::ParseResult const &result);
    float alphaBeta(Node const &node, int depth = 3, float alpha = -1e5, float beta = -1e5);
    std::string get_hint(Board const &main, std::vector<std::string> const &player,
            std::vector<std::string> const &enemy);

    void test_open();
    void test_hint();
}


#endif //TRIPLETRIAD_TRIPLETRIAD_H
