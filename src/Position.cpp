#include "TripleTriad.h"
#include "Position.h"
#include "Card.h"

void TripleTriad::Position::place(Card const &card) {
    _empty = false;
    _card = card;
    _card.place(_index);
    _card.checkElement(_element);
}

TripleTriad::Card const *TripleTriad::Position::card() const {
    if (_empty) return nullptr;
    return &_card;
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
    if (_card.team == team) return 0;

    _card.team = team;
    return 1;
}

