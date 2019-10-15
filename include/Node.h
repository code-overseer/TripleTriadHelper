//
// Created by Bryan Wong on 2019-10-15.
//

#ifndef TRIPLETRIAD_NODE_H
#define TRIPLETRIAD_NODE_H
#include "Utils.h"
#include "Board.h"
namespace TripleTriad {
    struct Node {
        Node(Node const &other);
        Node() = default;
        Node(std::string card, const std::vector<Card> &player, const std::vector<Card> &enemy,Board const &game);
        std::string name;
        int position = -1;
        float score = 0;
        Board board;
        std::unordered_map<Team, bool*> used = {{Blue, nullptr}, {Red, nullptr}};
        std::unordered_map<Team, int> size;
        std::unordered_map<Team, int> total;
        inline ~Node() { if (used.at(Blue)) delete [] used.at(Blue); if (used.at(Red)) delete [] used.at(Red); }
        inline Team turn() const { return board.turn(); }
        inline int turns(Team t) const { return size.at(t) - total.at(t); }
        Node forward(std::string const &card, int card_idx, int pos) const;
    };
}
#endif //TRIPLETRIAD_NODE_H
