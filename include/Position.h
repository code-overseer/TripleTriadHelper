#ifndef TRIPLETRIAD_POSITION_H
#define TRIPLETRIAD_POSITION_H
#include "TripleTriad.h"
#include "Card.h"

namespace TripleTriad {
    class Card;
    struct Position {
        Position() = default;
        explicit Position(int i, Position** root, Element e = None);
        Element element = None;
        void place(Card const &card);
        Card* getCard();
        Card const* card() const;
        inline std::vector<Position*> const& adjacent() const { return _adjacent; }
        inline void unplace() { _empty = true; }
        inline bool isEmpty() const { return _empty; }
        inline int getIdx() const { return _index; }
        bool isWall() const;
    private:
        Card _card;
        int _index = 0;
        bool _empty = true;
        std::vector<Position*> _adjacent;
    };

#define ASSIGN(OP) \
    bool operator OP(Position const& lhs, Position const& rhs);
ASSIGN(>)
ASSIGN(<)
ASSIGN(==)
#undef ASSIGN
    int operator+(Position const& lhs, Position const& rhs);
}

#endif //TRIPLETRIAD_POSITION_H
