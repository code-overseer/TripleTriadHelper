//
// Created by Bryan Wong on 2019-10-15.
//

#include "Node.h"

#include <utility>

TripleTriad::Node::Node(const TripleTriad::Node &other) : name(other.name), position(other.position),
score(other.score), board(other.board), total(other.total), size(other.size) {
    used.at(Blue) = other.used.at(Blue);
    used.at(Red) = other.used.at(Red);
}

TripleTriad::Node::Node(std::string card, const std::vector<Card> &player, const std::vector<Card> &enemy,
                        const TripleTriad::Board &game) : name(std::move(card)), board(game) {
    size.at(Blue) = player.size();
    size.at(Red) = enemy.size();
    total.at(Blue) = player.size();
    total.at(Red) = enemy.size();
    for (int i = 0; i < player.size(); ++i) used.at(Blue).emplace_back(0);
    for (int i = 0; i < enemy.size(); ++i) used.at(Red).emplace_back(0);
}

TripleTriad::Node TripleTriad::Node::forward(Card const &card, int card_idx, int pos, std::vector<Card> const &enemy) const {
    auto output = *this;
    output.name = name == "root" ? card.name() : name;
    output.position = name == "root" ? pos : position;
    output.score += static_cast<float>(output.board.play(card, pos));
    output.used.at(Blue)[card_idx] = 1;
    --output.total.at(Blue);
    int delta = 0;
    auto blanks = output.board.getBlanks();
    int max = 0;
    for (int i = 0; i < (int)enemy.size(); ++i) {
        if (output.used.at(Red)[i]) continue;
        for (auto const &blank : blanks) {
            delta = output.board.try_play(enemy[i], blank->idx());
            if (delta < max) max = delta;
        }
        output.score += static_cast<float>(delta) / total.at(Red);
    }
    ++output.depth;
    return output;
}

TripleTriad::Node TripleTriad::Node::red(int card_idx, std::vector<Card> const &enemy) {
    auto output = *this;
    int delta = 0;
    auto blanks = output.board.getBlanks();
    std::pair<int, int> max = { 0, 0 };
    for (auto const &blank : blanks) {
        delta = output.board.try_play(enemy[card_idx], blank->idx());
        if (delta < max.second) max = { blank->idx(), delta };
    }
    output.board.play(enemy[card_idx], max.first);
    output.used.at(Red)[card_idx] = 1;
    --output.total.at(Red);

    return output;
}

