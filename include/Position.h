#ifndef TRIPLETRIAD_POSITION_H
#define TRIPLETRIAD_POSITION_H
#include "TripleTriad.h"
#include "Card.h"

namespace TripleTriad {
    struct Position {
        Position() = default;
        explicit Position(int i, Element e = None) : _index(i), _element(e) {}
        Position(Position const &other);
        Position& operator=(Position &&other) noexcept;
        Position& operator=(Position const &other);
        int flip(Team team);
        void place(TripleTriad::Card const &card);
        TripleTriad::Card const* card() const;
        inline bool empty() const { return _empty; }
        inline int idx() const { return _index; }
        inline Element element() const { return _element; }
    private:
        Element _element = None;
        int _index = 0;
        TripleTriad::Card _card;
        bool _empty = true;
    };

}

#endif //TRIPLETRIAD_POSITION_H
