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
    _cardElement = card.element();
    _elemental = _element != None * ((_element == _cardElement) - (_element != _cardElement));
}

#define ASSIGN(OP) \
bool TripleTriad::operator OP(TripleTriad::Position const& lhs, TripleTriad::Position const& rhs) { \
    auto l = lhs.idx(); \
    auto r = rhs.idx(); \
    if (l / 3 != r / 3 && l % 3 != r % 3) return false; \
	switch (l - r) { \
		case 3: \
			return lhs.score(Up) + lhs._elemental OP rhs.score(Down) + rhs._elemental; \
		case -3: \
			return lhs.score(Down) + lhs._elemental OP rhs.score(Up) + rhs._elemental; \
		case 1: \
			return lhs.score(Left) + lhs._elemental OP rhs.score(Right) + rhs._elemental; \
		case -1: \
			return lhs.score(Right) + lhs._elemental OP rhs.score(Left) + rhs._elemental; \
		default: \
			return false; \
	} \
}
ASSIGN(>)
ASSIGN(<)
#undef ASSIGN

bool TripleTriad::operator==(TripleTriad::Position const& lhs, TripleTriad::Position const& rhs) {
    auto l = lhs.idx();
    auto r = rhs.idx();
    if (l / 3 != r / 3 && l % 3 != r % 3) return false;
	switch (l - r) {
		case 3:
			return lhs.score(Up) == rhs.score(Down);
		case -3:
			return lhs.score(Down) == rhs.score(Up);
		case 1:
			return lhs.score(Left) == rhs.score(Right);
		case -1:
			return lhs.score(Right) == rhs.score(Left);
		default:
			return false;
	}
}

int TripleTriad::operator+(TripleTriad::Position const& lhs, TripleTriad::Position const& rhs) {
    auto l = lhs.idx();
    auto r = rhs.idx();
    if (l / 3 != r / 3 && l % 3 != r % 3) return 0;
    switch (l - r) {
        case 3:
            return lhs.score(Up) + rhs.score(Down);
        case -3:
            return lhs.score(Down) + rhs.score(Up);
        case 1:
            return lhs.score(Left) + rhs.score(Right);
        case -1:
            return lhs.score(Right) + rhs.score(Left);
        default:
            return 0;
    }
}

bool TripleTriad::Position::wall() const {
    int v = _index / 3;
    int h = _index % 3;
    if (v == 0) return score(Up) == 10;
    if (v == 2) return score(Down) == 10;
    if (h == 0) return score(Left) == 10;
    if (h == 2) return score(Right) == 10;
    return false;
}
