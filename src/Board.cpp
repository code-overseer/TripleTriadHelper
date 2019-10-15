#include "Board.h"
#include "TripleTriad.h"

TripleTriad::Board::Board(Rules const &rules, std::string const &elements) {
    _same = rules.at(Same);
    _sameWall = rules.at(SameWall);
    _plus = rules.at(Plus);
    if (_elemental) {
        if (elements.size() != 9) throw std::runtime_error("Expected 9 elements");
        auto j = elements.begin();
        for (auto i = 0; i < 9; ++i, ++j) _pos[i] = Position(i, elementMap.at(elements[i]));
    } else {
        for (int i = 0; i < 9; ++i) _pos[i] = Position(i, None);
    }
    _computeAdjacents();
}

void TripleTriad::Board::_computeAdjacents() {
    static int adj[] = {3, -3, 1, -1};
    for (int i = 0; i < 9; ++i) {
        bool cond[4];
        int tmp = i / 3;
        cond[0] = tmp == 2;
        cond[1] = tmp == 0;
        tmp = i % 3;
        cond[2] = tmp == 2;
        cond[3] = tmp == 0;
        for (int j = 0; j < 4; ++j) {
            if (cond[j]) continue;
            auto idx = i + adj[j];
            _adjacent[i].emplace_back(&_pos[idx]);
        }
    }
}

std::vector<TripleTriad::Position*> TripleTriad::Board::_getSame(Card card, int position) const {
    card.place(_pos[position]);
    std::vector<TripleTriad::Position*> output;
    if (!_same) return output;
    int same_count = 0;
    for (auto const &i : _adjacent[position]) {
        if (i->empty() || !(card == *i->card())) continue;
        ++same_count;
        if (i->card()->team() != card.team()) output.emplace_back(i);
    }
    same_count += _sameWall && !output.empty() && card.isWall();
    if (same_count < 2) output.clear();
    _getCombo(position, output);
    return output;
}

std::vector<TripleTriad::Position*> TripleTriad::Board::_getPlus(Card card, int position) const {
    card.place(_pos[position]);
    std::vector<TripleTriad::Position*> output;
    if (!_plus) return output;
    std::set<Position*> set;
    auto &adj = _adjacent[position];
    int total[4] = {0,0,0,0};
    int k = 0;
    for (auto const &i : adj) {
        total[k++] = i->empty() ? 0 : card + *i->card();
    }
    for (int i = 0; i < 3; ++i) {
        if (!total[i]) continue;
        for (int j = i + 1; j < 4; ++j) {
            if (!total[j] || total[i] != total[j]) continue;
            if (adj[i]->card()->team() == card.team())
                set.emplace( adj[i] );
            if (adj[j]->card()->team() == card.team())
                set.emplace( adj[j] );
        }
    }
    output.assign( set.begin(), set.end() );
    _getCombo(position, output);
    return output;
}

std::vector<TripleTriad::Position*> TripleTriad::Board::_getDefaultFlips(Card card, int position, bool flip) const {
    if (flip) card.flip((Team)!card.team());
    card.place(_pos[position]);
    std::vector<TripleTriad::Position*> output;
    for (auto const &i : _adjacent[position]) {
        if (!i->empty() && card > *i->card() && card.team() != i->card()->team()) {
            output.emplace_back(i);
        }
    }
    return output;
}

void TripleTriad::Board::_getCombo(int position, std::vector<TripleTriad::Position*> &adjacents) const {
    if (adjacents.empty()) return;
    std::queue<Position*> bfs_queue;
    std::set<int> visited;
    for (auto const &i : adjacents) {
        bfs_queue.push(i);
        visited.emplace(i->idx());
    }
    visited.emplace(position);
    while (!bfs_queue.empty() && visited.size() != 9) {
        auto current = bfs_queue.front();
        auto combos = _getDefaultFlips(*(current->card()), current->idx(), true);
        auto const &played = *(current->card());
        bfs_queue.pop();
        for (auto const &i : combos) {
            if (visited.count(i->idx())) continue;
            visited.emplace(i->idx());
            if (i->card()->team() == current->card()->team()) {
                bfs_queue.push(i);
                adjacents.emplace_back(i);
            }
        }
    }
}

TripleTriad::Board::Board(const TripleTriad::Board &other) : _same(other._same), _sameWall(other._sameWall),
_elemental(other._elemental), _plus(other._plus), _score(other._score) {
    for (int i = 0; i < 9; ++i) _pos[i] = other._pos[i];
    _computeAdjacents();
}

int TripleTriad::Board::play(const TripleTriad::Card &card, int pos) {
    auto flips = _getFlips(card, pos);
    _pos[pos].place(card);
    _flip(flips, card.team());
    return flips.size();
}

std::set<TripleTriad::Position*> TripleTriad::Board::_getFlips(const TripleTriad::Card &card, int position) const {
    std::vector<Position*> flips;
    std::set<Position*> output;
    flips = _getSame(card, position);
    output.insert(flips.begin(), flips.end());
    flips = _getPlus(card, position);
    output.insert(flips.begin(), flips.end());
    flips = _getDefaultFlips(card, position);

    output.insert(flips.begin(), flips.end());
    return output;
}
void TripleTriad::Board::_flip(std::set<Position *> const &positions, Team team) {
    for (auto const &i : positions) {
        static int j = 0;
        j = i->flip(team);
        _score[team] += j;
        _score[team == Red ? Blue : Red] -= j;
    }
}

float TripleTriad::Board::_computeHint(Card const &card, int pos, std::vector<Card> const &enemy) {
    auto state = *this;
    auto score = (float)state.play(card, pos);
    auto blanks = state.getBlanks();
    auto enemy_card = enemy.empty() ? Card::cardList() : &enemy[0];
    auto size = enemy.empty() ? 109.0 : static_cast<double>(enemy.size());
    for (int c = 0; c < size; ++c, ++enemy_card) {
        float max = 0;
        for (auto const &i : blanks) {
            auto s = (float)state._getFlips(*enemy_card, i->idx()).size();
            if (s > max) max = s;
        }
        score -= max / size;
    }
    return score;
}

std::vector<TripleTriad::Position const*> TripleTriad::Board::getBlanks() const {
    std::vector<Position const*> blanks;
    for (int i = 0; i < 9; ++i) {
        if (_pos[i].empty()) blanks.emplace_back(_pos + i);
    }
    return blanks;
}

std::string TripleTriad::Board::hint(std::vector<Card> const &player, std::vector<Card> const &enemy) {
    static struct compare {
        bool operator() (hint_t const &i, hint_t const &j) { return (i.score > j.score);}
    } c;

    std::stringstream out_stream;
    auto blanks = getBlanks();
    std::vector<hint_t> hints;
    for (auto const &blank_pos : blanks) {
        for (auto const &card : player) {
            hints.emplace_back((hint_t){card.name(), blank_pos->idx(), _computeHint(card, blank_pos->idx(), enemy) });
        }
    }
    std::sort(hints.begin(), hints.end(), c);

    out_stream << std::left << std::setw(6) << std::setfill(' ') << "No.";
    out_stream << std::left << std::setw(15) << std::setfill(' ') << "Card";
    out_stream << std::left << std::setw(9) << std::setfill(' ') << "Position";
    out_stream << std::left << std::setw(15) << std::setfill(' ') << "Potential";
    out_stream << '\n';

    for (int i = 0; i < 9 && i < (int)hints.size(); ++i) {
        out_stream << std::left << std::setw(6) << std::setfill(' ') << i + 1;
        out_stream << std::left << std::setw(15) << std::setfill(' ') << hints[i].name;
        out_stream << std::left << std::setw(9) << std::setfill(' ') << hints[i].position;
        out_stream << std::left << std::setw(15) << std::setfill(' ') << hints[i].score;
        out_stream << '\n';
    }

    return out_stream.str();
}






