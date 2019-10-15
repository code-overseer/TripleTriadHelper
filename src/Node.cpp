//
// Created by Bryan Wong on 2019-10-15.
//

#include "Node.h"

#include <utility>

TripleTriad::Node::Node(const TripleTriad::Node &other) : name(other.name), position(other.position),
score(other.score), board(other.board), total(other.total), size(other.size) {
    used.at(Blue) = new bool[size.at(Blue)];
    used.at(Red) = new bool[size.at(Red)];
    for (int i = 0; i < size.at(Blue); ++i) {
        used.at(Blue)[i] = other.used.at(Blue)[i];
    }
    for (int i = 0; i < size.at(Red); ++i) {
        used.at(Red)[i] = other.used.at(Red)[i];
    }
}

TripleTriad::Node::Node(std::string card, const std::vector<Card> &player, const std::vector<Card> &enemy,
                        const TripleTriad::Board &game) : name(std::move(card)), board(game) {
    used.at(Blue) = new bool[size.at(Blue)];
    used.at(Red) = new bool[size.at(Red)];
    memset(used.at(Blue), 0, player.size());
    memset(used.at(Red), 0, enemy.size());
    size.at(Blue) = player.size();
    size.at(Red) = enemy.size();
    total.at(Blue) = player.size();
    total.at(Red) = enemy.size();
}

TripleTriad::Node TripleTriad::Node::forward(std::string const &card, int card_idx, int pos) const {
    auto output = *this;
    auto turn = board.turn();
    output.name = card;
    output.position = pos;
    output.score += static_cast<float>(output.board.play(name, pos)) / (turn == Red ? total.at(Red) : 1);
    output.used.at(turn)[card_idx] = true;
    --output.total.at(turn);

    return output;
}
