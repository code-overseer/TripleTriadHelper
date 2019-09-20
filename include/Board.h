#ifndef TRIPLETRIAD_BOARD_H
#define TRIPLETRIAD_BOARD_H

#include "TripleTriad.h"
#include "Position.h"
#include "Card.h"

namespace TripleTriad {
    class Card;
    class Board {
        struct hint_t {
            std::string name;
            int position = -1;
            float score = 0;
        };
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
        std::vector<Position const*> _getBlanks() const;
        void _getCombo(int position, std::vector<Position*> &adjacents) const;
        void _computeAdjacents();
        std::set<Position*> _getFlips(Card const &card, int position) const;
        void _flip(std::set<Position*> const &positions, Team team);
        float _computeHint(TripleTriad::Card const &card, int pos, std::vector<Card> const &enemy);
    public:
        Board(Rules const &rules, std::string const &elements);
        Board(Board const &other);
        void hint(std::vector<Card> const &player, std::vector<Card> const &enemy = std::vector<Card>());
        int play(Card const &card, int pos);
        inline Card const* card(int i) const { return _pos[i].card(); }
        inline Element element(int i) const { return _pos[i].element(); }
        inline bool isElemental(int i) const { return !_pos[i].empty() && _pos[i].element() == _pos[i].card()->element(); }
    };
}

#endif //TRIPLETRIAD_BOARD_H
