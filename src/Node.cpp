#include "Node.h"
#include "Utils.h"

TripleTriad::Branch::Node::Node(int first_card, int pos, std::vector<std::string> const &player,
                  std::vector<std::string> const &enemy, Board main) : uid(first_card), board(std::move(main)), position(pos) {
    stats = {{Blue, stat_t(player)}, {Red, stat_t(enemy)} };
    auto &stat = stats.at(board.turn());
    stat.isUsed[first_card] = true;
    auto d = heuristic(board.play(stat.cards[0][first_card], pos));

    if (board.turn() == Blue && stat.delta + d < -4)
        value = -10;
    else
        value += d;

    stat.delta = d;
}

TripleTriad::Branch::Node::Node(Node const &other) = default;

TripleTriad::Branch::Node::Node(Node &&other) noexcept : board(std::move(other.board)), uid(other.uid),
                                                         position(other.position), value(other.value), stats(std::move(other.stats)) {}

TripleTriad::Branch::Node &TripleTriad::Branch::Node::operator=(const TripleTriad::Branch::Node &other) = default;

TripleTriad::Branch::Node &TripleTriad::Branch::Node::operator=(Node &&other) noexcept {
    board = std::move(other.board);
    stats = std::move(other.stats);
    uid = other.uid;
    position = other.position;
    value = other.value;
    return *this;
}

TripleTriad::Branch::Node TripleTriad::Branch::Node::forward(int card_idx, int blank) const {
    auto output = *this;
    auto &stat = output.stats.at(board.turn());
    stat.isUsed[card_idx] = true;
    auto d = output.heuristic(output.board.play(stat.cards[0][card_idx], blank));
    if (board.turn() == Blue && stat.delta + d < -4)
        output.value = -10;
    else
        output.value += d;

    stat.delta = d;
    return output;
}

float TripleTriad::Branch::Node::heuristic(int gain) {
    auto output = static_cast<float>(gain);
    auto blanks = board.getBlanks();
    auto &stat = stats.at(board.turn());
    int num = stat.remaining() * static_cast<int>(blanks.size());
    int loss[num];
    memset(loss, 0, num * sizeof(int));
    int n = 0;
    for (auto const &blank : blanks) {
        for (int i = 0; i < stat.size(); ++i, ++n) {
            if (stat.isUsed[i]) continue;
            loss[n] = board.calculate(stat.cards[0][i], blank->idx());
            if (loss[n] < -2) {
                output = -10;
                break;
            }
        }
    }
    output -= static_cast<float>(std::accumulate(loss, loss + num, 0)) / static_cast<float>(num);

    return output;
}
