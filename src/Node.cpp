#include "Node.h"

#include <utility>

TripleTriad::Node::Node(int first_card, int pos, std::vector<std::string> const &player,
                  std::vector<std::string> const &enemy, Board main) : _uid(first_card), _board(std::move(main)), _position(pos) {
    _stats = { {Blue, stat_t(player)}, {Red, stat_t(enemy)} };
}

TripleTriad::Node::Node(TripleTriad::Node const &other) : _board(other._board), _uid(other._uid),
_position(other._position), _value(other._value), _stats(other._stats) {}

TripleTriad::Node::Node(TripleTriad::Node &&other) noexcept : _board(std::move(other._board)), _uid(other._uid),
_position(other._position), _value(other._value), _stats(std::move(other._stats)) {

}

TripleTriad::Node &TripleTriad::Node::operator=(const TripleTriad::Node &other) {
    _board = other._board;
    _uid = other._uid;
    _position = other._position;
    _value = other._value;
    _stats = other._stats;
    return *this;
}

TripleTriad::Node &TripleTriad::Node::operator=(TripleTriad::Node &&other) noexcept {
    _board = std::move(other._board);
    _stats = std::move(other._stats);
    _uid = other._uid;
    _position = other._position;
    _value = other._value;
    return *this;
}

TripleTriad::Node TripleTriad::Node::forward(int card_idx, int pos) const {
    auto output = *this;
    auto &stat = output._stats.at(turn());
    stat.isUsed[card_idx] = true;
    auto d = output._heuristic(output._board.play(stat.cards[0][card_idx], pos));
    if (turn() == Blue && stat.delta + d < -4)
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
    memset(loss, 0, num * sizeof(int));
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
