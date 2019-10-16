#ifndef TRIPLETRIAD_BOARD_H
#define TRIPLETRIAD_BOARD_H

#include "Utils.h"
#include "Position.h"

namespace TripleTriad {
    class Board {
        Rules _rules;
        Team _turn = Blue;
        Score _score = { {Red, 5}, {Blue, 5} };
        std::vector<Position> _pos = std::vector<Position>(static_cast<size_t>(9));
        std::vector<std::vector<Position*>> _adj;
        void _adjacent();
    public:
        Board() = default;
        Board(Rules &&rules, std::string const &elements, Team turn);
        Board(Board const &other);
        inline int const &score(Team team) const { return _score.at(team); }
        inline Team const &turn() const { return _turn; }
    };
}

#endif //TRIPLETRIAD_BOARD_H
