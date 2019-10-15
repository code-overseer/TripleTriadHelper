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
        Node(std::string card, const std::vector<Card> &player, const std::vector<Card> &enemy,Board const &game);
        std::string name;
        int position = -1;
        int depth = 0;
        Board board;
        std::unordered_map<Team, std::vector<int>> used = {{Blue, std::vector<int>()}, {Red, std::vector<int>()}};
        std::unordered_map<Team, int> size = {{Blue, 0}, {Red, 0}};
        std::unordered_map<Team, int> total = {{Blue, 0}, {Red, 0}};
//        std::vector<float> score;
        float score = 0;
        inline Team turn() const { return board.turn(); }
        inline int turns(Team t) const { return size.at(t) - total.at(t); }
        inline float value() const { return score; }
        Node forward(const Card &card, int card_idx, int pos, std::vector<Card> const &enemy) const;
        Node red(int card_idx, std::vector<Card> const &enemy);
    };
}
#endif //TRIPLETRIAD_NODE_H
