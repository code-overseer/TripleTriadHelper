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
        std::vector<std::list<Position*>> _adj;
        std::set<Position*> _getFlips(std::string const &card_name, int pos);
        void _getDefaultFlips(std::set<Position *> &flips, int pos) const;
        void _getComboFlips(std::set<Position *> &flips, int pos, bool visited[9]) const;
        void _getSameFlips(std::set<Position *> &flips, int pos) const;
        void _getPlusFlips(std::set<Position *> &flips, int pos) const;
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
