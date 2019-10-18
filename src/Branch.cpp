#include "Branch.h"
#include "Node.h"

TripleTriad::Branch::Branch(int first_card, int pos, std::vector<std::string> const &player,
                            std::vector<std::string> const &enemy, Board const &main) : pos(pos), card(first_card) {
    val = _compute(Node(first_card, pos, player, enemy, main));
}

float TripleTriad::Branch::_compute(const Node &node, int depth, float alpha, float beta) {
    if (!depth || node.value < -5 || node.terminated()) return node.value;
    float value = -1e5;
    auto blanks = node.blanks();
    auto cards = static_cast<int>(node.cards(Blue).size());
    for (auto const &blank : blanks) {
        for (int i = 0; i < cards; ++i) {
            if (node.used(node.turn(), i)) continue;
            value = std::max(value, _compute(node.forward(i, blank->idx()), depth - 1, alpha, beta));
            if (node.turn() == Blue) {
                alpha = std::max(alpha, value);
            } else {
                beta = std::max(beta, value);
            }
            if (alpha >= -beta) break;
        }
    }
    return value;
}



