#include "Card.h"

#include <utility>

std::unordered_map<std::string, int> TripleTriad::Card::_cardFinder;
TripleTriad::Card TripleTriad::Card::_cardDB[109];
std::unordered_map<char, Element> const TripleTriad::Card::_elementMap = // NOLINT(cert-err58-cpp)
        {{'N', None}, {'T', Thunder}, {'E', Earth}, {'P', Poison}, {'F', Fire},
         {'I', Ice}, {'A', Water}, {'W', Wind}, {'H', Holy}};

void TripleTriad::Card::checkElement(Element pos_element) {
    if (pos_element == None) return;
    reset();
    if (pos_element == _element) {
        for (auto &i : _effectiveScore) i++;
    } else {
        for (auto &i : _effectiveScore) i--;
    }
}

TripleTriad::Card::Card(TripleTriad::Card &&other) noexcept : _name(std::move(other._name)), _element(other._element) {
    memcpy(_defaultScore, other._defaultScore, 4 * sizeof(int));
    memcpy(_effectiveScore, other._effectiveScore, 4 * sizeof(int));
    _team = other._team;
    _defaultTeam = other._defaultTeam;
}

TripleTriad::Card::Card(std::string card_name, Element element, int score[4]) : _name(std::move(card_name)), _element(element) {
    memcpy(_defaultScore, score, 4 * sizeof(int));
    memcpy(_effectiveScore, score, 4 * sizeof(int));
}

TripleTriad::Card &TripleTriad::Card::operator=(TripleTriad::Card &&other) noexcept {
    _name = std::move(other._name);
    _element = other._element;
    memcpy(_defaultScore, other._defaultScore, 4 * sizeof(int));
    memcpy(_effectiveScore, other._effectiveScore, 4 * sizeof(int));
    _team = other._team;
    _defaultTeam = other._defaultTeam;
    _idx = other._idx;
    return *this;
}

TripleTriad::Card &TripleTriad::Card::operator=(const TripleTriad::Card &other) {
    _name = other._name;
    _element = other._element;
    memcpy(_defaultScore, other._defaultScore, 4 * sizeof(int));
    memcpy(_effectiveScore, other._effectiveScore, 4 * sizeof(int));
    _team = other._team;
    _defaultTeam = other._defaultTeam;
    _idx = other._idx;
    return *this;
}

TripleTriad::Card TripleTriad::Card::Factory(char const *card_name, Team team) {
    if (_cardFinder.empty()) {
        io::CSVReader<6> data(CARD_DATA);
        std::string name;
        int score[4];
        char e;
        int i = 0;
        while (data.read_row(name, score[0], score[1], score[2], score[3], e)) {
            _cardDB[i] = Card(name, _elementMap.at(e), score);
            _cardFinder.insert({ _cardDB[i].name(), i });
            ++i;
        }
    }
    auto out = _cardDB[_cardFinder.at(card_name)];
    out._defaultTeam = team;
    out._team = team;

    return out;
}
bool TripleTriad::Card::isWall() const {
    if (_idx != 4) {
        auto tmp = _idx / 3;
        if (tmp == 2 && s() == 10) return true;
        if (!tmp && n() == 10) return true;
        tmp = _idx % 3;
        if (tmp == 2 && e() == 10) return true;
        if (!tmp && w() == 10) return true;
    }
    return false;
}
#define ASSIGN(OP) \
bool TripleTriad::operator OP(TripleTriad::Card const& lhs, TripleTriad::Card const& rhs) { \
	switch (lhs._idx - rhs._idx) { \
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
    switch (lhs._idx - rhs._idx) {
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
    switch (lhs._idx - rhs._idx) {
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
