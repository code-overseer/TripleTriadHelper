#include "Board.h"
#include "Position.h"

TripleTriad::Board::Board(Rules const &rules, const Elements &elements) {
    std::unordered_map<Rule, bool&> rule_map = {{Same, _same}, {SameWall, _sameWall}, {Plus, _plus}, {Elemental, _elemental}};
    for (auto const &i : rules) rule_map.at(i) = true;
    if (_elemental) {
        auto j = elements.begin();
        for (auto i = 0; i < 9; ++i, ++j) _pos[i] = Position(i, *j);
    } else {
        for (int i = 0; i < 9; ++i) _pos[i] = Position(i, None);
    }
    _computeAdjacents();
}

void TripleTriad::Board::_computeAdjacents() {
    static int adj[] = {3, -3, 1, -1};
    for (int i = 0; i < 9; ++i) {
        int tmp = i / 3;
        bool cond[4];
        cond[0] = tmp == 2;
        cond[1] = tmp == 0;
        tmp = i % 3;
        cond[2] = tmp == 0;
        cond[3] = tmp == 2;
        for (int j = 0; j < 4; ++j) {
            if (cond[j]) continue;
            auto idx = i + adj[j];
            _adjacent[i].emplace_back(&_pos[idx]);
        }
    }
}

void TripleTriad::Board::_defaultFlip(int position) {
    auto flips = _getDefaultFlips(*_pos[position].card(), position);
    auto team = _pos[position].card()->team;
    for (auto &i : flips) _score[team] += i->flip(team);
}

void TripleTriad::Board::_sameFlip(int position) {
    auto sames = _getSame(*_pos[position].card(), position);
    auto team = _pos[position].card()->team;
    for (auto const &i : sames) _score[team] += i->flip(team);
}

void TripleTriad::Board::_plusFlip(int position) {
    auto pluses = _getPlus(*_pos[position].card(), position);
    auto team = _pos[position].card()->team;
    for (auto const &i : pluses) _score[team] += i->flip(team);
}

std::vector<TripleTriad::Position*> TripleTriad::Board::_getSame(Card card, int position) const {
    card.place(position);
    std::vector<TripleTriad::Position*> output;
    if (!_same) return output;
    for (auto const &i : _adjacent[position]) {
        if (i->isEmpty()) continue;
        if (card == *i->card()) output.emplace_back(i);
    }
    if (output.size() < 2 && !(_sameWall && output.size() == 1 && card.isWall())) output.clear();
    _getCombo(position, output);
    return output;
}

std::vector<TripleTriad::Position*> TripleTriad::Board::_getPlus(Card card, int position) const {
    card.place(position);
    std::vector<TripleTriad::Position*> output;
    if (!_plus) return output;
    std::set<Position*> set;
    auto &adj = _adjacent[position];
    int total[4];
    for (auto const &i : adj) {
        static int j = 0;
        total[j++] = card + *i->card();
    }
    for (int i = 0; i < 3; ++i) {
        if (!total[i]) continue;
        for (int j = i + 1; j < 4; ++j) {
            if (!total[j]) continue;
            if (total[i] == total[j]) {
                set.emplace( adj[i] );
                set.emplace( adj[j] );
            }
        }
    }
    output.assign( set.begin(), set.end() );
    if (output.size() < 2) output.clear();
    _getCombo(position, output);
    return output;
}

std::vector<TripleTriad::Position*> TripleTriad::Board::_getDefaultFlips(Card card, int position) const {
    card.place(position);
    std::vector<TripleTriad::Position*> output;
    for (auto const &i : _adjacent[position]) {
        if (i->isEmpty()) continue;
        if (card > *i->card()) output.emplace_back(i);
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
        auto combos = _getDefaultFlips(*(current->card()), current->idx());
        auto const &played = *(current->card());
        bfs_queue.pop();
        for (auto const &i : combos) {
            if (visited.count(i->idx())) continue;
            visited.emplace(i->idx());
            bfs_queue.push(i);
            adjacents.emplace_back(i);
        }
    }
}

TripleTriad::Board::Board(const TripleTriad::Board &other) : _same(other._same), _sameWall(other._sameWall),
_elemental(other._elemental), _plus(other._plus), _score(other._score) {
    for (int i = 0; i < 9; ++i) _pos[i] = other._pos[i];
    _computeAdjacents();
}

void TripleTriad::Board::play(const TripleTriad::Card &card, int pos) {
    _pos[pos].place(card);
    _defaultFlip(pos);
    _sameFlip(pos);
    _plusFlip(pos);
}

void TripleTriad::Board::check(const TripleTriad::Card &card, int pos) {

}



