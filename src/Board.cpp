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
}

TripleTriad::Board::~Board() {
    for (int i = 0; i < 9; ++i) delete _pos[i];
    delete [] _pos;
}

void TripleTriad::Board::_checkDefault(int position) {
    auto const &played = *(_pos[position]->card());
    for (auto &i : _getCombo(position)) {
        auto other = i->getCard();
        if (*_pos[position] > *i) other->flip(played);
    }
}

void TripleTriad::Board::_checkSame(int position) {
    auto sames = _getSame(position);
    if (sames.size() < 2 && !(_sameWall && sames.size() == 1 && _pos[position]->isWall())) return;
    auto const &played = *(_pos[position]->card());
    for (auto const &i : sames) {
        if (i->isEmpty()) continue;
        auto other = i->getCard();
        other->flip(played);
    }
}

void TripleTriad::Board::_checkPlus(int position) {
    auto pluses = _getPlus(position);

    if (pluses.size() < 2) return;
    auto const &played = *(_pos[position]->card());
    for (auto const &i : pluses) {
        auto other = i->getCard();
        other->flip(played);
    }
}

void TripleTriad::Board::_checkCombo(int position, std::vector<Position *> const &flipped) {
    std::queue<Position*> bfs_queue;
    for (auto const &i : flipped) bfs_queue.push(i);
    std::set<Position*> visited(flipped.begin(), flipped.end());
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
            i->getCard()->flip(played);
        }
    }
}

std::vector<TripleTriad::Position*> TripleTriad::Board::_getSame(int position) const {
    std::vector<TripleTriad::Position*> output;
    for (auto const &i : _pos[position]->adjacent()) {
        if (i->isEmpty()) continue;
        if (*_pos[position] == *i) output.emplace_back(i);
    }
    return output;
}

std::vector<TripleTriad::Position*> TripleTriad::Board::_getPlus(int position) const {
    std::set<Position*> set;
    auto &adj = _pos[position]->adjacent();
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
    for (auto const &i : _pos[position]->adjacent()) {
        if (i->isEmpty()) continue;
        if (*_pos[position] > *i) output.emplace_back(i);
    }
    return output;
}



