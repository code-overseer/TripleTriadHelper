#include "Node.h"

TripleTriad::Node::Node(int first_card, int pos, std::vector<std::string> const &player,
                  std::vector<std::string> const &enemy, Board const &main) : _uid(first_card), _board(main), _position(pos) {
    _stats = { {Blue, stat_t(player)}, {Red, stat_t(enemy)} };
    forward(first_card, pos);
}

TripleTriad::Node TripleTriad::Node::forward(int card_idx, int pos) const {
    auto output = *this;
    output._depth++;
    auto &stat = output._stats.at(turn());
    stat.isUsed[card_idx] = true;
    auto d = output._heuristic(output._board.play(stat.cards[0][card_idx], pos));
    if (stat.delta < 0.1 && d < 0.1)
        output._value = -10;
    else
        output._value += d;

    stat.delta = d;
    return output;
}

float TripleTriad::Node::_heuristic(int gain) {
    auto output = static_cast<float>(gain);
    auto blanks = _board.getBlanks();
    auto &stat = _stats.at(turn());
    int num = stat.remaining() * static_cast<int>(blanks.size());
    int loss[num];
    int n = 0;
    for (auto const &blank : blanks) {
        for (int i = 0; i < stat.size(); ++i, ++n) {
            if (stat.isUsed[i]) continue;
            loss[n] = _board.calculate(stat.cards[0][i], blank->idx());
            if (loss[n] < -2) {
                output = -10;
                break;
            }
        }
    }
    output -= static_cast<float>(std::accumulate(loss, loss + num, 0)) / static_cast<float>(num);

    return output;
}
