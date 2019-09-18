#include "csv.h"
#include "Card.h"
#include "Position.h"

TripleTriad::Card::Card(TripleTriad::Card &&other) noexcept : _name(std::move(other._name)), _element(other._element) {
    memcpy(_score, other._score, 4 * sizeof(int));
    _team = other._team;
}

TripleTriad::Card::Card(std::string card_name, Element element, int score[4]) : _name(std::move(card_name)), _element(element) {
    memcpy(_score, score, 4 * sizeof(int));
}

TripleTriad::Card &TripleTriad::Card::operator=(TripleTriad::Card &&other) noexcept {
    _name = std::move(other._name);
    _element = other._element;
    memcpy(_score, other._score, 4 * sizeof(int));
    _team = other._team;
    return *this;
}

TripleTriad::Card &TripleTriad::Card::operator=(const TripleTriad::Card &other) {
    _name = other._name;
    _element = other._element;
    memcpy(_score, other._score, 4 * sizeof(int));
    _team = other._team;
    _position = other._position;
    return *this;
}

TripleTriad::Card::Card(char const *card_name, Team team) {
    *this = cardFinder(card_name);
    _team = team;
}

void TripleTriad::Card::place(TripleTriad::Position const &pos) { _position = &pos; }

int TripleTriad::Card::idx() const { return _position ? _position->idx() : -1; }

bool TripleTriad::Card::isWall() const {
    if (idx() != 4) {
        auto tmp = idx() / 3;
        if (tmp == 2 && s() == 10) return true;
        if (!tmp && n() == 10) return true;
        tmp = idx() % 3;
        if (tmp == 2 && e() == 10) return true;
        if (!tmp && w() == 10) return true;
    }
    return false;
}
#define ASSIGN(OP) \
bool TripleTriad::operator OP(TripleTriad::Card const& lhs, TripleTriad::Card const& rhs) { \
	switch (lhs.idx() - rhs.idx()) { \
		case 3: \
			return lhs.n() OP rhs.s(); \
		case -3: \
			return lhs.s() OP rhs.n(); \
		case 1: \
			return lhs.w() OP rhs.e(); \
		case -1: \
			return lhs.e() OP rhs.w(); \
		default: \
			throw std::runtime_error("Not adjacent positions"); \
	} \
}
ASSIGN(>)
ASSIGN(<)
#undef ASSIGN

bool TripleTriad::operator==(TripleTriad::Card const& lhs, TripleTriad::Card const& rhs) {
    switch (lhs.idx() - rhs.idx()) {
        case 3:
            return lhs.n(true) == rhs.s(true);
        case -3:
            return lhs.s(true) == rhs.n(true);
        case 1:
            return lhs.w(true) == rhs.e(true);
        case -1:
            return lhs.e(true) == rhs.w(true);
        default:
            throw std::runtime_error("Not adjacent cards");
    }
}

int TripleTriad::operator+(TripleTriad::Card const& lhs, TripleTriad::Card const& rhs) {
    switch (lhs.idx() - rhs.idx()) {
        case 3:
            return lhs.n(true) + rhs.s(true);
        case -3:
            return lhs.s(true) + rhs.n(true);
        case 1:
            return lhs.w(true) + rhs.e(true);
        case -1:
            return lhs.e(true) + rhs.w(true);
        default:
            throw std::runtime_error("Not adjacent cards");
    }
}

int TripleTriad::Card::n(bool isDefault) const {
    if (isDefault || _position->element() == None) return _score[0];
    bool a = _position->element() == _element;
    return _score[0] + a - !a;
}

int TripleTriad::Card::s(bool def) const {
    if (def || _position->element() == None) return _score[1];
    bool a = _position->element() == _element;
    return _score[1] + a - !a;
}

int TripleTriad::Card::e(bool def) const {
    if (def || _position->element() == None) return _score[2];
    bool a = _position->element() == _element;
    return _score[2] + a - !a;
}

int TripleTriad::Card::w(bool def) const {
    if (def || _position->element() == None) return _score[3];
    bool a = _position->element() == _element;
    return _score[3] + a - !a;
}

TripleTriad::Card const *TripleTriad::Card::cardList(int i) {
    static Card database[109];
    static bool init = false;
    if (!init) {
        init = true;
        io::CSVReader<6> data(CARD_DATA);
        std::string name;
        int score[4];
        char e;
        int j = 0;
        while (data.read_row(name, score[0], score[1], score[2], score[3], e)) {
            database[j] = Card(name, elementMap.at(e), score);
            ++j;
        }
    }
    return database + i;
}

std::unordered_map<std::string, TripleTriad::Card const*> const& TripleTriad::Card::cardFinder() {
    static std::unordered_map<std::string, Card const*> finder;
    static bool init = false;
    if (!init) {
        init = true;
        for (int i = 0; i < 109; ++i) {
            auto c = cardList(i);
            finder.insert({ c->name(), c });
        }
    }
    return finder;
}

TripleTriad::Card const& TripleTriad::Card::cardFinder(std::string const &name) {
    static auto map = cardFinder();
    return *map.at(name);
}