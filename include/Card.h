#ifndef TRIPLETRIAD_CARD_H
#define TRIPLETRIAD_CARD_H

#include "TripleTriad.h"

namespace TripleTriad {
    class Position;
    class Card {
        std::string _name;
        int _score[4] = {0, 0, 0, 0};
        Element _element = None;
        Position const* _position = nullptr;
        Team _team = Red;
        Card(std::string card_name, Element element, int score[4]);
    public:
        static Card const* cardList(int i = 0);
        static std::unordered_map<std::string, Card const*> const& cardFinder();
        static Card const& cardFinder(std::string const &name);
        Card() = default;
        Card(char const* card_name, Team team);
        Card(Card const &other) = default;
        Card(Card &&other) noexcept;
        bool isWall() const;
        int idx() const;
        void place(Position const &pos);
        int n(bool isDefault = false) const;
        int s(bool isDefault = false) const;
        int e(bool isDefault = false) const;
        int w(bool isDefault = false) const;
        inline Element const &element() const { return _element; }
        inline Team const &team() const { return _team; }
        inline std::string const& name() const { return _name; }
        inline void flip(Team team) { _team = team; }
        Card& operator=(Card const &other);
        Card& operator=(Card &&other) noexcept;
        friend void TripleTriad::test_open();
        #define ASSIGN(OP) \
        friend bool operator OP(Card const& lhs, Card const& rhs);
        ASSIGN(>)
        ASSIGN(<)
        ASSIGN(==)
        #undef ASSIGN
        friend int operator+(Card const& lhs, Card const& rhs);
    };

}

#endif //TRIPLETRIAD_CARD_H
