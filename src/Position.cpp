#include "TripleTriad.h"
#include "Position.h"
#include "Card.h"

void TripleTriad::Position::place(Card const &card) {
    _empty = false;
    _card = card;
    _card.place(_index);
    _card.checkElement(_element);
}

//TripleTriad::Card *TripleTriad::Position::getCard() {
//    if (_empty) return nullptr;
//    return &_card;
//}

TripleTriad::Card const *TripleTriad::Position::card() const {
    if (_empty) return nullptr;
    return &_card;
}

bool TripleTriad::Position::isWall() const {
    if (_index != 4 && !_empty) {
        auto tmp = _index / 3;
        if (tmp == 2 && _card.s() == 10) return true;
        if (!tmp && _card.n() == 10) return true;
        tmp = _index % 3;
        if (tmp == 2 && _card.e() == 10) return true;
        if (!tmp && _card.w() == 10) return true;
    }
    return false;
}

TripleTriad::Position &TripleTriad::Position::operator=(TripleTriad::Position &&other) noexcept {
    _index = other._index;
    _card = other._card;
    _element = other._element;
    _empty = other._empty;

    return *this;
}

int TripleTriad::Position::flip(Team team) {
    if (_empty) return 0;
    if (_card.team != team) {
        _card.team = team;
        return 1;
    }
    return 0;
}
