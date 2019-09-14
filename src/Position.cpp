#include "TripleTriad.h"
#include "Position.h"
#include "Card.h"

TripleTriad::Position::Position(int i, TripleTriad::Position** root, Element e)  : _index(i), element(e) {
    using namespace TripleTriad;
    static int adj[] = {3, -3, 1, -1};
    int tmp = i / 3;
    bool cond[4];
    cond[0] = tmp == 2;
    cond[1] = tmp == 0;
    tmp = i % 3;
    cond[2] = tmp == 0;
    cond[3] = tmp == 2;
    for (int j = 0; j < 4; ++j) {
        if (cond[j]) continue;
        auto idx = i + adj[j];
        _adjacent.emplace_back(root[idx]);
    }
}

TripleTriad::Card *TripleTriad::Position::getCard() {
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

#define ASSIGN(OP) \
bool TripleTriad::operator OP(TripleTriad::Position const& lhs, TripleTriad::Position const& rhs) { \
    if (lhs.isEmpty() || rhs.isEmpty()) return false; \
	switch (lhs.getIdx() - rhs.getIdx()) { \
		case 3: \
			return lhs.card().n() OP rhs.card().s(); \
		case -3: \
			return lhs.card().s() OP rhs.card().n(); \
		case 1: \
			return lhs.card().w() OP rhs.card().e(); \
		case -1: \
			return lhs.card().e() OP rhs.card().w(); \
		default: \
			throw std::runtime_error("Not adjacent positions"); \
	} \
}
ASSIGN(>)
ASSIGN(<)
#undef ASSIGN

bool TripleTriad::operator==(TripleTriad::Position const& lhs, TripleTriad::Position const& rhs) {
    if (lhs.isEmpty() || rhs.isEmpty()) return false;
    switch (lhs.getIdx() - rhs.getIdx()) {
        case 3:
            return lhs.card().n(true) == rhs.card().s(true);
        case -3:
            return lhs.card().s(true) == rhs.card().n(true);
        case 1:
            return lhs.card().w(true) == rhs.card().e(true);
        case -1:
            return lhs.card().e(true) == rhs.card().w(true);
        default:
            throw std::runtime_error("Not adjacent cards");
    }
}


int TripleTriad::operator+(TripleTriad::Position const& lhs, TripleTriad::Position const& rhs) {
    if (lhs.isEmpty() || rhs.isEmpty()) return 0;
    switch (lhs.getIdx() - rhs.getIdx()) {
        case 3:
            return lhs.card().n(true) + rhs.card().s(true);
        case -3:
            return lhs.card().s(true) + rhs.card().n(true);
        case 1:
            return lhs.card().w(true) + rhs.card().e(true);
        case -1:
            return lhs.card().e(true) + rhs.card().w(true);
        default:
            throw std::runtime_error("Not adjacent cards");
    }
}