#ifndef TRIPLETRIAD_POSITION_H
#define TRIPLETRIAD_POSITION_H

#include "Utils.h"
#include "Card.h"
namespace TripleTriad {
    struct Position {
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
    private:
        inline void _flip() { _team = _team == Red ? Blue : Red; }

        inline void _clear() { _empty = true; };

        inline void _confirm() { _empty = false; };

        Position &operator=(Position &&other) noexcept = default;

        Position &operator=(Position const &other) = default;

        Position() = default;

        Position(Position const &other) = default;

        Position(Position &&other) noexcept = default;

        explicit Position(int i, Element e = None) : _index(i), _element(e) {}

        int const *_score = nullptr;
        std::string _name;
        Element _element = None;
        int _index = -1;
        bool _empty = true;
        Team _team = Blue;
        int _elemental = 0;
    };
}

#endif //TRIPLETRIAD_POSITION_H
