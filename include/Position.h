#ifndef TRIPLETRIAD_POSITION_H
#define TRIPLETRIAD_POSITION_H

#include "Utils.h"
#include "Card.h"
#include "GUI.h"

namespace TripleTriad {
    struct Position {
        struct PositionAllocator: std::allocator<Position> {
            template< class U, class... Args >
            void construct( U* p, Args&&... args ) {
                ::new((void *)p) U(std::forward<Args>(args)...);
            }
            template< class U > struct rebind { typedef PositionAllocator other; };
        };
        friend class PositionAllocator;
        void place(std::string const &card_name, Team team);
        void check(std::string const &card_name, Team team);
        bool wall() const;
        inline bool const &empty() const { return _empty; }
        inline int const &idx() const { return _index; }
        inline Element const &element() const { return _element; }
        inline Team const &team() const { return _team; }
        inline int const &score(Direction dir) const { return _score[dir]; }
        friend class Board;
        #define ASSIGN(OP) \
        friend bool operator OP(TripleTriad::Position const& lhs, TripleTriad::Position const& rhs);
        ASSIGN(>)
        ASSIGN(<)
        ASSIGN(==)
        #undef ASSIGN
        friend int operator+(TripleTriad::Position const& lhs, TripleTriad::Position const& rhs);
        friend void GUI::drawBlock(std::pair<int, int> const &root, TripleTriad::Position const &pos);
    private:
        Position() = default;
        Position(Position const &other) = default;
        Position(Position &&other) noexcept = default;
        explicit Position(int i, Element e = None) : _index(i), _element(e) {}
        inline void _flip() { _team = _team == Red ? Blue : Red; };;
        Position &operator=(Position &&other) noexcept = default;
        Position &operator=(Position const &other) = default;
        int const *_score = nullptr;
        std::string _name;
        Element _element = None;
        Element _cardElement = None;
        int _index = -1;
        bool _empty = true;
        Team _team = Blue;
        int _elemental = 0;
    };
}

#endif //TRIPLETRIAD_POSITION_H
