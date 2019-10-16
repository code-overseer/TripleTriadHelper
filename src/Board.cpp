#include "Board.h"
#include "TripleTriad.h"

TripleTriad::Board::Board(Rules &&rules, std::string const &elements, Team turn) : _turn(turn), _rules(std::move(rules)) {
    if (rules.at(Elemental)) {
        if (elements.size() != 9) throw std::runtime_error("Expected 9 elements");
        auto j = elements.begin();
        for (auto i = 0; i < 9; ++i, ++j) _pos.emplace_back(i, elementMap.at(elements[i]));
    } else {
        for (int i = 0; i < 9; ++i) _pos.emplace_back(i, None);
    }
    _adjacent();
}

TripleTriad::Board::Board(const TripleTriad::Board &other) : _rules(other._rules), _turn(other._turn),
                                                             _score(other._score), _pos(other._pos) {
    _adjacent();
}

void TripleTriad::Board::_adjacent() {
    _adj = {{_pos.data() + 1, _pos.data() + 3},
             {_pos.data() + 0, _pos.data() + 2, _pos.data() + 4},
             {_pos.data() + 1, _pos.data() + 5},
             {_pos.data() + 0, _pos.data() + 4, _pos.data() + 6},
             {_pos.data() + 1, _pos.data() + 3, _pos.data() + 5, _pos.data() + 7},
             {_pos.data() + 2, _pos.data() + 4, _pos.data() + 8},
             {_pos.data() + 3, _pos.data() + 7},
             {_pos.data() + 4, _pos.data() + 6, _pos.data() + 8},
             {_pos.data() + 5, _pos.data() + 7}};
}

std::set<TripleTriad::Position *> TripleTriad::Board::_getFlips(std::string const &card_name, int pos) {
    auto output = std::set<Position *>();
    _pos[pos].check(card_name, _turn);
    _getDefaultFlips(output, pos);
    _getSameFlips(output, pos);
    _getPlusFlips(output, pos);

    return output;
}

void TripleTriad::Board::_getDefaultFlips(std::set<Position *> &flips, int pos) const {
    for (auto const &adj : _adj[pos]) {
        if (adj->empty() || adj->team() == _turn || !(_pos[pos] > *adj)) continue;
        flips.emplace(adj);
    }
}

void TripleTriad::Board::_getComboFlips(std::set<Position *> &flips, int pos, bool visited[9]) const {
    if (visited[pos]) return;
    visited[pos] = true;
    for (auto const &adj : _adj[pos]) {
        if (adj->empty() || adj->team() != _turn || !(_pos[pos] > *adj)) continue;
        flips.emplace(adj);
        _getComboFlips(flips, adj->idx(), visited);
    }
}

void TripleTriad::Board::_getSameFlips(std::set<Position *> &flips, int pos) const {
    if (!_rules.at(Same)) return;
    std::list<Position*> same(_adj[pos].size());
    for (auto const &adj : _adj[pos]) {
        if (adj->empty() || adj->team() == _turn || !(_pos[pos] == *adj)) continue;
        same.emplace_back(adj);
    }
    if (same.size() > 1 || (_rules.at(SameWall) && same.size() == 1 && _pos[pos].wall())) {
        bool visited[9];
        memset(visited, 0, 9);
        for (auto const &card : same) {
            _getComboFlips(flips, card->idx(), visited);
        }
    }
}

void TripleTriad::Board::_getPlusFlips(std::set<Position *> &flips, int pos) const {
    if (!_rules.at(Plus)) return;
    std::list<Position*> plus(_adj[pos].size());
    int x = 1;
    for (auto const &i : _adj[pos]) {
        int y = 0;
        for (auto const &j : _adj[pos]) {
            if (y++ < x || i->empty() || j->empty() || i->team() == _turn ||
            j->team() == _turn || _pos[pos] + *j != _pos[pos] + *i) continue;

            plus.emplace_back(i);
            plus.emplace_back(j);
        }
        ++x;
    }
    bool visited[9];
    memset(visited, 0, 9);
    for (auto const &card : plus) {
        _getComboFlips(flips, card->idx(), visited);
    }
}







