#include "TripleTriad.h"
#include "Position.h"
#include "Card.h"

void TripleTriad::Position::place(Card const &card) {
    _empty = false;
    _card = card;
    _card.place(*this);
}

TripleTriad::Position::Position(TripleTriad::Position const &other) : _empty(other._empty), _element(other._element),
 _index(other._index), _card(other._card) {
    _card.place(*this);
}

TripleTriad::Card const *TripleTriad::Position::card() const {
    if (_empty) return nullptr;
    return &_card;
}

TripleTriad::Position &TripleTriad::Position::operator=(TripleTriad::Position const &other) {
    _index = other._index;
    _card = other._card;
    _element = other._element;
    _empty = other._empty;
    _card.place(*this);
    return *this;
}

TripleTriad::Position &TripleTriad::Position::operator=(TripleTriad::Position &&other) noexcept {
    _index = other._index;
    _card = other._card;
    _element = other._element;
    _empty = other._empty;
    _card.place(*this);
    return *this;
}

int TripleTriad::Position::flip(Team team) {
    if (_empty) return 0;
    if (_card.team() == team) return 0;
    _card.flip(team);
    return 1;
}

