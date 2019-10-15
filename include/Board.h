#ifndef TRIPLETRIAD_BOARD_H
#define TRIPLETRIAD_BOARD_H

#include "Utils.h"
#include "Position.h"
#include "Card.h"

namespace TripleTriad {
    class Card;
    class Board {
        bool _same = false;
        bool _plus = false;
        bool _sameWall = false;
        bool _elemental = false;
        Team _turn = Blue;
        Score _score = { {Red, 5}, {Blue, 5} };
        Position _pos[9];
        std::vector<Position*> _adjacent[9];
        std::vector<Position*> _getSame(Card card, int position) const;
        std::vector<Position*> _getPlus(Card card, int position) const;
        std::vector<Position*> _getDefaultFlips(Card card, int position, bool combo = false) const;
        void _getCombo(int position, std::vector<Position*> &adjacents) const;
        void _computeAdjacents();
        std::set<Position*> _getFlips(const Card &card, int position) const;
        void _flip(std::set<Position*> const &positions);
    public:
        Board() = default;
        Board(Rules const &rules, std::string const &elements, Team turn);
        Board(Board const &other);
        std::vector<Position const*> getBlanks() const;
        int play(std::string const &card, int pos);
        inline int score(Team team) const { return _score.at(team); }
        inline Card const* card(int i) const { return _pos[i].card(); }
        inline Element element(int i) const { return _pos[i].element(); }
        inline Team turn() const { return _turn; }
        inline bool isElemental(int i) const { return !_pos[i].empty() && _pos[i].element() == _pos[i].card()->element(); }
    };
}

#endif //TRIPLETRIAD_BOARD_H
