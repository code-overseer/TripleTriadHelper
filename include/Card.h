#ifndef TRIPLETRIAD_CARD_H
#define TRIPLETRIAD_CARD_H

#include "TripleTriad.h"
#include "csv.h"

namespace TripleTriad {
    class Card {
        int _defaultNum[4] = {0, 0, 0, 0 };
        int _effectiveNum[4] = {0, 0, 0, 0};
        Element _element = None;
        void flip(Card const &flipper) { team = flipper.team; }
        inline void reset() { memcpy(_effectiveNum, _defaultNum, 4 * sizeof(int)); }
        static std::unordered_map<char, Element> const _elementMap;
        static io::CSVReader<6> _cardData;
        std::string _name;
    public:
        Team team = Blue;
        void checkElement(Element pos_element);
        static float const northProb[10];
        static float const southProb[10];
        static float const eastProb[10];
        static float const westProb[10];
        static std::unordered_map<Element, float> const elementProb;
        inline std::string const& name() const { return _name; }
        int const& n(bool def = false) const { return def ? _defaultNum[0] : _effectiveNum[0]; }
        int const& s(bool def = false) const { return def ? _defaultNum[1] : _effectiveNum[1]; }
        int const& e(bool def = false) const { return def ? _defaultNum[2] : _effectiveNum[2]; }
        int const& w(bool def = false) const { return def ? _defaultNum[3] : _effectiveNum[3]; }
        Card() = default;
        Card(char const* card_name, Team team);
        friend class Board;
    };
}

#endif //TRIPLETRIAD_CARD_H
