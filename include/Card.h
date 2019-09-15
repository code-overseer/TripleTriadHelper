#ifndef TRIPLETRIAD_CARD_H
#define TRIPLETRIAD_CARD_H

#include "TripleTriad.h"
#include "csv.h"

namespace TripleTriad {
    class Card {
        static std::unordered_map<char, Element> const _elementMap;
        static io::CSVReader<6> _cardData;
        int _defaultNum[4] = {0, 0, 0, 0 };
        int _effectiveNum[4] = {0, 0, 0, 0};
        int _idx = -1;
        Element _element = None;
        std::string _name;
        inline void reset() { memcpy(_effectiveNum, _defaultNum, 4 * sizeof(int)); }
    public:
        static float const northProb[10];
        static float const southProb[10];
        static float const eastProb[10];
        static float const westProb[10];
        static std::unordered_map<Element, float const> const elementProb;
        Team team = Blue;

        void checkElement(Element pos_element);
        inline void place(int i) { if (i < 9 && i >= 0) _idx = i; }
        inline void unplace() { _idx = -1; }
        inline std::string const& name() const { return _name; }
        int const& n(bool def = false) const { return def ? _defaultNum[0] : _effectiveNum[0]; }
        int const& s(bool def = false) const { return def ? _defaultNum[1] : _effectiveNum[1]; }
        int const& e(bool def = false) const { return def ? _defaultNum[2] : _effectiveNum[2]; }
        int const& w(bool def = false) const { return def ? _defaultNum[3] : _effectiveNum[3]; }
        Card() = default;
        Card(char const* card_name, Team team);
        Card(Card const &other) = default;
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
