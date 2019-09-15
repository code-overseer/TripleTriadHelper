#include "Board.h"
#include "Position.h"

TripleTriad::Board::Board(Rules const &rules, const Elements &elements) {
    std::unordered_map<Rule, bool&> rule_map = {{Same, _same}, {SameWall, _sameWall}, {Plus, _plus}, {Elemental, _elemental}};
    for (auto const &i : rules) rule_map.at(i) = true;
    if (_elemental) {
        auto j = elements.begin();
        for (auto i = 0; i < 9; ++i, ++j) _pos[i] = new Position(i, _pos, *j);
    } else {
        for (int i = 0; i < 9; ++i) _pos[i] = new Position(i, _pos);
    }
    _computeAdjacents();
}

TripleTriad::Board::~Board() {
    for (auto & p : _pos) delete p;
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
            _adjacent[i].emplace_back(_pos[idx]);
        }
    }
}

void TripleTriad::Board::_defaultFlip(int position) {
    auto const &played = *(_pos[position]->card());
    for (auto &i : _getCombo(position)) {
        auto other = i->getCard();
        if (*_pos[position] > *i && other->team != played.team) {
            other->flip(played);
            _score[played.team] += 1;
        }
    }
}

void TripleTriad::Board::_sameFlip(int position) {
    auto sames = _getSame(position);
    if (sames.size() < 2 && !(_sameWall && sames.size() == 1 && _pos[position]->isWall())) return;
    auto const &played = *(_pos[position]->card());
    for (auto const &i : sames) {
        if (i->isEmpty()) continue;
        auto other = i->getCard();
        if (other->team != played.team) {
            other->flip(played);
            _score[played.team] += 1;
        }
    }
    _comboFlip(position, sames);
}

void TripleTriad::Board::_plusFlip(int position) {
    auto pluses = _getPlus(position);

    if (pluses.size() < 2) return;
    auto const &played = *(_pos[position]->card());
    for (auto const &i : pluses) {
        auto other = i->getCard();
        if (other->team != played.team) {
            other->flip(played);
            _score[played.team] += 1;
        }
    }
    _comboFlip(position, pluses);
}

void TripleTriad::Board::_comboFlip(int position, std::vector<Position *> const &adjacents) {
    std::queue<Position*> bfs_queue;
    for (auto const &i : adjacents) bfs_queue.push(i);
    std::set<Position*> visited(adjacents.begin(), adjacents.end());
    visited.emplace(_pos[position]);
    while (!bfs_queue.empty() && visited.size() != 9) {
        auto current = bfs_queue.front();
        auto combos = _getCombo(current->getIdx());
        auto const &played = *(current->card());
        bfs_queue.pop();
        for (auto const &i : combos) {
            if (visited.count(i)) continue;
            visited.emplace(i);
            bfs_queue.push(i);
            if (i->card()->team == played.team) continue;
            i->getCard()->flip(played);
            _score[played.team] += 1;
        }
    }
}

std::vector<TripleTriad::Position*> TripleTriad::Board::_getSame(int position) const {
    std::vector<TripleTriad::Position*> output;
    for (auto const &i : _adjacent[position]) {
        if (i->isEmpty()) continue;
        if (*_pos[position] == *i) output.emplace_back(i);
    }
    return output;
}

std::vector<TripleTriad::Position*> TripleTriad::Board::_getPlus(int position) const {
    std::set<Position*> set;
    auto &adj = _adjacent[position];
    int total[4];
    for (auto const &i : adj) {
        static int j = 0;
        total[j++] = *_pos[position] + *i;
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
    std::vector<TripleTriad::Position*> output;
    output.assign( set.begin(), set.end() );
    return output;
}

std::vector<TripleTriad::Position*> TripleTriad::Board::_getCombo(int position) const {
    std::vector<TripleTriad::Position*> output;
    for (auto const &i : _adjacent[position]) {
        if (i->isEmpty()) continue;
        if (*_pos[position] > *i) output.emplace_back(i);
    }
    return output;
}



