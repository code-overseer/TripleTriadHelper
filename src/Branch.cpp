#include "Branch.h"
#include "Node.h"

TripleTriad::Branch::Branch(int first_card, int pos, std::vector<std::string> const &player,
                            std::vector<std::string> const &enemy, Board const &main) : pos(pos), card(first_card) {
    val = alphabeta(Node(first_card, pos, player, enemy, main));
}

float TripleTriad::Branch::alphabeta(const Node &node, int depth, float alpha, float beta) {
    auto blanks = node.blanks();

    auto cards = static_cast<int>(node.cards(node.turn()).size());
    if (!depth || node.value < 0 || blanks.empty()) return node.value;
    if (node.turn() == Blue) {
        float value = -50;
        for (auto const &blank : blanks) {
            for (int i = 0; i < cards; ++i) {
                if (node.used(node.turn(), i)) continue;
                value = std::max(value, alphabeta(node.forward(i, blank->idx()), depth - 1, alpha, beta));
                alpha = std::max(alpha, value);
                if (alpha >= beta) break;
            }
        }
        return value;
    }
    float value = 50;
    for (auto const &blank : blanks) {
        for (int i = 0; i < cards; ++i) {
            if (node.used(node.turn(), i)) continue;
            value = std::min(value, alphabeta(node.forward(i, blank->idx()), depth - 1, alpha, beta));
            beta = std::min(beta, value);
            if (alpha >= beta) break;
        }
    }
    return value;

}



