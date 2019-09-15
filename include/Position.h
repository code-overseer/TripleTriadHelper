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
        inline Card unplace() { _empty = true; _card.unplace(); return _card; }
        int flip(Team team);
        Card const* card() const;
        bool isWall() const;
        bool isWall(Card const &card) const;
        inline bool isEmpty() const { return _empty; }
        inline int idx() const { return _index; }
        inline Element element() const { return _element; }
    private:
        Element _element = None;
        int _index = 0;
        Card _card;
        bool _empty = true;
    };

}

#endif //TRIPLETRIAD_POSITION_H
