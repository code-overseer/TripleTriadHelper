#ifndef TRIPLETRIAD_CARD_H
#define TRIPLETRIAD_CARD_H

#include "TripleTriad.h"
#include "Position.h"
#include "csv.h"

namespace TripleTriad {
    class Card {
        static std::unordered_map<char, Element> const _elementMap;
        static Card _cardDB[109];
        static std::unordered_map<std::string, Card*> _cardFinder;
        int _defaultScore[4] = {0, 0, 0, 0};
        int _effectiveScore[4] = {0, 0, 0, 0};
        int _idx = -1;
        Element _element = None;
        std::string _name;
        Team _defaultTeam = Blue;
        Team _team = Blue;
        inline void reset() { memcpy(_effectiveScore, _defaultScore, 4 * sizeof(int)); }
        Card(char const* card_name, Element element, int score[4]);
    public:
        static Card Factory(char const* card_name, Team team);
        Card() = default;
        Card(Card const &other) = default;
        void checkElement(Element pos_element);
        bool isWall() const;
        inline Team const &team() const { return _team; }
        inline void place(int i) { if (i < 9 && i >= 0) _idx = i; }
        inline void unplace() { _idx = -1; _team = _defaultTeam; }
        inline std::string const& name() const { return _name; }
        inline int const& n(bool def = false) const { return def ? _defaultScore[0] : _effectiveScore[0]; }
        inline int const& s(bool def = false) const { return def ? _defaultScore[1] : _effectiveScore[1]; }
        inline int const& e(bool def = false) const { return def ? _defaultScore[2] : _effectiveScore[2]; }
        inline int const& w(bool def = false) const { return def ? _defaultScore[3] : _effectiveScore[3]; }
        inline void flip(Team team) { _team = team; }
        Card& operator=(Card const &other);
        friend class Board;
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
