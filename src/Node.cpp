#include "Node.h"
#include "Utils.h"

TripleTriad::Branch::Node::Node(int first_card, int pos, std::vector<std::string> const &player,
                  std::vector<std::string> const &enemy, Board main) : uid(first_card), board(std::move(main)), position(pos) {
    stats = {{Blue, stat_t(player)}, {Red, stat_t(enemy)} };
    auto &stat = stats.at(board.turn());
    stat.isUsed[first_card] = true;
    board.play(stat.cards[0][first_card], pos);
    heuristic();
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

TripleTriad::Branch::Node TripleTriad::Branch::Node::spawn(int card_idx, int blank) const {
    auto output = *this;
    auto &stat = output.stats.at(board.turn());
    stat.isUsed[card_idx] = true;
    output.board.play(stat.cards[0][card_idx], blank);
    output.heuristic();
    return output;
}

void TripleTriad::Branch::Node::heuristic() {
    auto blanks = board.getBlanks();
    if (blanks.empty()) {
        value = static_cast<float>(board.score(Blue));
        return;
    }
    auto &stat = stats.at(board.turn());
    int num = stat.remaining() * static_cast<int>(blanks.size());
    int loss = 0;
    int n = 0;
    for (auto const &blank : blanks) {
        for (int i = 0; i < stat.size(); ++i, ++n) {
            if (stat.isUsed[i]) continue;
            auto tmp = board.calculate(stat.cards[0][i], blank->idx());
            if (tmp < -3) { value = -10; break; }
            loss += tmp;
        }
    }
    value = static_cast<float>(board.score(Blue)) + (static_cast<float>(loss) / static_cast<float>(num));
}
