#ifndef TRIPLETRIAD_BOARD_H
#define TRIPLETRIAD_BOARD_H

#include "TripleTriad.h"

namespace TripleTriad {
    class Position;
    class Card;
    class Board {
        bool _same = false;
        bool _plus = false;
        bool _sameWall = false;
        bool _elemental = false;
        Position* _pos[9];
        Score _score = { {Red, 5}, {Blue, 5} };
        std::vector<Position*> _adjacent[9];

        std::vector<Position*> _getSame(int position) const;
        std::vector<Position*> _getPlus(int position) const;
        std::vector<Position*> _getCombo(int position) const;
        void _computeAdjacents();
        void _defaultFlip(int position);
        void _sameFlip(int position);
        void _plusFlip(int position);
        void _comboFlip(int position, std::vector<Position*> const &adjacents);
    public:
        Board(Rules const &rules, Elements const &elements = Elements());
        void play(Card const &card, int pos);
        ~Board();
    };
}

#endif //TRIPLETRIAD_BOARD_H
