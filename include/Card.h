#ifndef TRIPLETRIAD_CARD_H
#define TRIPLETRIAD_CARD_H

#include "Utils.h"

namespace TripleTriad {
    class Position;
    class Card {
        std::string _name;
        int _score[4] = {0, 0, 0, 0};
        Element _element = None;
        Card(std::string card_name, Element element, int score[4]);
        static Card const* cardList(int i = 0);
    public:
        static Card const& getCard(std::string const &name);
        Card(Card const &other) = delete;
        Card(Card &&other) = delete;
        inline Element const &element() const { return _element; }
        inline std::string const& name() const { return _name; }
        inline int const* const score() const { return _score; }
        inline int up() const { return _score[0]; }
        inline int down() const { return _score[1]; }
        inline int right() const { return _score[2]; }
        inline int left() const { return _score[3]; }
    };

}

#endif //TRIPLETRIAD_CARD_H
