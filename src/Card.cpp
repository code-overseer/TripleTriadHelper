#include "Card.h"

float const TripleTriad::Card::northProb[10] = {0.064220183, 0.082568807, 0.082568807, 0.100917431, 0.137614679,
                                                0.155963303, 0.155963303, 0.100917431, 0.064220183, 0.055045872};
float const TripleTriad::Card::southProb[10] = {0.082568807, 0.128440367, 0.110091743, 0.155963303, 0.110091743,
                                                0.110091743, 0.110091743, 0.091743119, 0.055045872, 0.04587156};
float const TripleTriad::Card::eastProb[10] = {0.111111111, 0.12037037, 0.092592593, 0.111111111, 0.12962963,
                                               0.138888889, 0.083333333, 0.12037037, 0.027777778, 0.064814815};
float const TripleTriad::Card::westProb[10] = {0.04587156, 0.100917431, 0.183486239, 0.165137615, 0.119266055,
                                               0.091743119, 0.100917431, 0.073394495, 0.055045872, 0.064220183};
std::unordered_map<Element, float const> const TripleTriad::Card::elementProb = // NOLINT(cert-err58-cpp)
        {{None, 0.669724771}, {Thunder, 0.064220183}, {Earth, 0.064220183}, {Poison, 0.055045872}, {Fire, 0.064220183},
         {Ice, 0.027522936}, {Water, 0.018348624}, {Wind, 0.027522936}, {Holy, 0.009174312}};

std::unordered_map<char, Element> const TripleTriad::Card::_elementMap = // NOLINT(cert-err58-cpp)
        {{'N', None}, {'T', Thunder}, {'E', Earth}, {'P', Poison}, {'F', Fire},
         {'I', Ice}, {'A', Water}, {'W', Wind}, {'H', Holy}};

io::CSVReader<6> TripleTriad::Card::_cardData(CARD_DATA); // NOLINT(cert-err58-cpp)

void TripleTriad::Card::checkElement(Element pos_element) {
    if (pos_element == None) return;
    reset();
    if (pos_element == _element) {
        for (auto &i : _effectiveNum) i++;
    } else {
        for (auto &i : _effectiveNum) i--;
    }
}

TripleTriad::Card::Card(char const *card_name, Team team) : team(team), _default(team) {
    _name.reserve(24);
    char e; bool found = false;
    while(_cardData.read_row(_name, _defaultNum[0], _defaultNum[1], _defaultNum[2], _defaultNum[3], e) && !found) {
        if (_name != card_name) continue;
        memcpy(_effectiveNum, _defaultNum, 4 * sizeof(int));
        _element = _elementMap.at(e);
        found = true;
    }
    if (!found) throw std::runtime_error("Could not find card");
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