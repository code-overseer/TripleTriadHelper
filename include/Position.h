//
// Created by Bryan Wong on 2019-09-14.
//

#ifndef TRIPLETRIAD_POSITION_H
#define TRIPLETRIAD_POSITION_H
#include "TripleTriad.h"
#include "Card.h"

namespace TripleTriad {
    struct Position {
        Position() = default;
        explicit Position(int i, Position** root, Element e = None);
        Element element = None;
        inline void place(Card card) { _card = card; _empty = false; }
        inline void unplace() { _card = Card(); _empty = true; }
        inline std::vector<Position*>& adjacent() { return _adjacent; }
        Card* getCard();
        inline Card const& card() const { return _card; }
        inline bool isEmpty() const { return _empty; }
        inline int getIdx() const { return _index; }
        bool isWall() const;
        #define ASSIGN(OP) \
	        friend bool operator OP(Position const& lhs, Position const& rhs);
        ASSIGN(>)
        ASSIGN(<)
        ASSIGN(==)
        #undef ASSIGN
        friend int operator+(Position const& lhs, Position const& rhs);
    private:
        Card _card;
        int _index = 0;
        bool _empty = true;
        std::vector<Position*> _adjacent;
    };

}
#endif //TRIPLETRIAD_POSITION_H
