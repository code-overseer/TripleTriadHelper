#ifndef TRIPLETRIAD_BOARD_H
#define TRIPLETRIAD_BOARD_H

#include "TripleTriad.h"
#include "Position.h"

namespace TripleTriad {
    class Card;
    class Board {
        bool _same = false;
        bool _plus = false;
        bool _sameWall = false;
        bool _elemental = false;
        Score _score = { {Red, 5}, {Blue, 5} };
        Position _pos[9];
        int _cardCount = 0;
        std::vector<Position*> _adjacent[9];
        std::vector<Position*> _getSame(Card card, int position) const;
        std::vector<Position*> _getPlus(Card card, int position) const;
        std::vector<Position*> _getDefaultFlips(Card card, int position) const;
        void _getCombo(int position, std::vector<Position*> &adjacents) const;
        void _computeAdjacents();
        std::set<Position*> _getFlips(Card const &card, int position) const;
        void _flip(std::set<Position*> const &positions, Team team);
    public:
        Board(Rules const &rules, Elements const &elements = Elements());
        Board(Board const &other);
        int play(Card const &card, int pos);
        float check(const TripleTriad::Card &card, int pos, std::vector<Card> const &enemy = std::vector<Card>());
    };
}

#endif //TRIPLETRIAD_BOARD_H
