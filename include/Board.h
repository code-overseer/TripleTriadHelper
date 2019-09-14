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
        Position** _pos = new Position*[9];
        Score _score = { {Red, 5}, {Blue, 5} };

        std::vector<Position*> _getSame(int position) const;
        std::vector<Position*> _getPlus(int position) const;
        std::vector<Position*> _getCombo(int position) const;
        void _checkDefault(int position);
        void _checkSame(int position);
        void _checkPlus(int position);
        void _checkCombo(int position, std::vector<Position*> const &flipped);
    public:
        Board(Rules const &rules, Elements const &elements = Elements());
        void play(Card const &card, int pos);
        ~Board();

    };
}

#endif //TRIPLETRIAD_BOARD_H
