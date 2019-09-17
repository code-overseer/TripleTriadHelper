#ifndef TRIPLETRIAD_CARD_H
#define TRIPLETRIAD_CARD_H

#include "TripleTriad.h"
#include "csv.h"

namespace TripleTriad {
    class Position;
    class Card {
        static std::unordered_map<char, Element> const _elementMap;
        static Card _cardDB[109];
        static std::unordered_map<std::string, int> _cardFinder;
        std::string _name;
        int _score[4] = {0, 0, 0, 0};
        Element _element = None;
        Position const* _position = nullptr;
        Team _team = Red;
        Card(std::string card_name, Element element, int score[4]);
    public:
        static Card Factory(char const* card_name, Team team);
        static Card const* Try() { return _cardDB; }
        Card() = default;
        Card(Card const &other) = default;
        Card(Card &&other) noexcept;
        bool isWall() const;
        int idx() const;
        void place(Position const &pos);
        int n(bool def = false) const;
        int s(bool def = false) const;
        int e(bool def = false) const;
        int w(bool def = false) const;
        inline Team const &team() const { return _team; }
        inline std::string const& name() const { return _name; }
        inline void flip(Team team) { _team = team; }
        Card& operator=(Card const &other);
        Card& operator=(Card &&other) noexcept;
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
