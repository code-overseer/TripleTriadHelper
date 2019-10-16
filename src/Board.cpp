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

