#ifndef TRIPLETRIAD_CARD_H
#define TRIPLETRIAD_CARD_H

#include "TripleTriad.h"

namespace TripleTriad {
    class Card {
        Team team = Blue;
        int _defaultNum[4] = {0, 0, 0, 0 };
        int _effectiveNum[4] = {0, 0, 0, 0};
        Element _element = None;
        void checkElement(Element pos_element);
        void flip(Card const &flipper) { team = flipper.team; }
        inline void reset() { memcpy(_effectiveNum, _defaultNum, 4 * sizeof(int)); }
    public:
        int const& n(bool def = false) const { return def ? _defaultNum[0] : _effectiveNum[0]; }
        int const& s(bool def = false) const { return def ? _defaultNum[1] : _effectiveNum[1]; }
        int const& e(bool def = false) const { return def ? _defaultNum[2] : _effectiveNum[2]; }
        int const& w(bool def = false) const { return def ? _defaultNum[3] : _effectiveNum[3]; }
        Card() = default;
        Card(std::initializer_list<int> att, Team team, Element e = None);
        friend class Board;
    };
}

#endif //TRIPLETRIAD_CARD_H
