#ifndef TRIPLETRIAD_POSITION_H
#define TRIPLETRIAD_POSITION_H
#include "TripleTriad.h"
#include "Card.h"

namespace TripleTriad {
    class Card;
    struct Position {
        Position() = default;
        explicit Position(int i, Element e = None) : _index(i), _element(e) {}
        Position(Position const &other) = default;
        Position& operator=(Position &&other) noexcept;
        Position& operator=(Position const &other) = default;
        void place(Card const &card);
        int flip(Team team);
        Card const* card() const;
        inline Card unplace() { _empty = true; _card.place(-1); return _card; }
        inline bool isEmpty() const { return _empty; }
        inline int getIdx() const { return _index; }
        bool isWall() const;
        inline Element element() { return _element; }
    private:
        Element _element = None;
        int _index = 0;
        Card _card;
        bool _empty = true;
    };

}

#endif //TRIPLETRIAD_POSITION_H
