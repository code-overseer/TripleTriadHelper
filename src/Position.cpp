#include "TripleTriad.h"
#include "Position.h"
#include "Card.h"

void TripleTriad::Position::place(std::string const &card_name, Team team) {
    check(card_name, team);
    _empty = false;
}

void TripleTriad::Position::check(std::string const &card_name, Team team) {
    if (!_empty) throw std::runtime_error("Position already occupied");
    _team = team;
    if (card_name == _name) return;

    _name = card_name;
    auto const &card = Card::getCard(card_name);
    _score = card.score();
    _elemental = _element != None * ((_element == card.element()) - (_element != card.element()));
}
