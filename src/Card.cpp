#include "Card.h"

void TripleTriad::Card::checkElement(Element pos_element) {
    if (pos_element == None) return;
    if (pos_element == _element) {
        for (auto &i : _effectiveNum) i++;
    } else {
        for (auto &i : _effectiveNum) i--;
    }
}

TripleTriad::Card::Card(std::initializer_list<int> att, Team team, Element e) : _element(e), team(team) {
    if (att.size() != 4) throw std::invalid_argument("Must have 4 values per card");
    for (auto const& i : att) {
        static int j = 0;
        _defaultNum[j++] = i;
    }
    memcpy(_effectiveNum, _defaultNum, 4 * sizeof(int));
}


