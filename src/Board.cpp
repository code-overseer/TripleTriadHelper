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
    auto const &played = *(_pos[position].card());
    for (auto &i : _getDefaultFlips(position)) {
        _score[played.team] += i->flip(played.team);
    }
}

void TripleTriad::Board::_sameFlip(int position) {
    auto sames = _getSame(position);
    auto const &played = *(_pos[position].card());
    for (auto const &i : sames) {
        _score[played.team] += i->flip(played.team);
    }
    _comboFlip(position, sames);
}

void TripleTriad::Board::_plusFlip(int position) {

    auto pluses = _getPlus(position);

    auto const &played = *(_pos[position].card());
    for (auto const &i : pluses) {
        _score[played.team] += i->flip(played.team);
    }
    _comboFlip(position, pluses);
}

void TripleTriad::Board::_comboFlip(int position, std::vector<TripleTriad::Position *> const &adjacents) {
    auto combos = _getCombo(position, adjacents);
    auto const &played = *(_pos[position].card());
    for (auto const &i : combos) {
        _score[played.team] += i->flip(played.team);
    }
}

std::vector<TripleTriad::Position*> TripleTriad::Board::_getSame(int position) const {
    std::vector<TripleTriad::Position*> output;
    if (!_same) return output;
    for (auto const &i : _adjacent[position]) {
        if (i->isEmpty()) continue;
        if (*(_pos[position].card()) == *(i->card())) output.emplace_back(i);
    }
    if (output.size() < 2 && !(_sameWall && output.size() == 1 && _pos[position].isWall())) output.clear();
    return output;
}

std::vector<TripleTriad::Position*> TripleTriad::Board::_getPlus(int position) const {
    std::vector<TripleTriad::Position*> output;
    if (!_plus) return output;
    std::set<Position*> set;
    auto &adj = _adjacent[position];
    int total[4];
    for (auto const &i : adj) {
        static int j = 0;
        total[j++] = *(_pos[position].card()) + *(i->card());
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
    return output;
}

std::vector<TripleTriad::Position*> TripleTriad::Board::_getDefaultFlips(int position) const {
    std::vector<TripleTriad::Position*> output;
    for (auto const &i : _adjacent[position]) {
        if (i->isEmpty()) continue;
        if (*(_pos[position].card()) > *(i->card())) output.emplace_back(i);
    }
    return output;
}

std::vector<TripleTriad::Position *> TripleTriad::Board::_getCombo(int position, std::vector<TripleTriad::Position*> const &adjacents) const {
    std::vector<TripleTriad::Position*> output;
    if (adjacents.empty()) return output;
    std::queue<Position*> bfs_queue;
    std::set<int> visited;
    for (auto const &i : adjacents) {
        bfs_queue.push(i);
        visited.emplace(i->getIdx());
    }
    visited.emplace(position);
    while (!bfs_queue.empty() && visited.size() != 9) {
        auto current = bfs_queue.front();
        auto combos = _getDefaultFlips(current->getIdx());
        auto const &played = *(current->card());
        bfs_queue.pop();
        for (auto const &i : combos) {
            if (visited.count(i->getIdx())) continue;
            visited.emplace(i->getIdx());
            bfs_queue.push(i);
            output.emplace_back(i);
        }
    }
    return output;
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



