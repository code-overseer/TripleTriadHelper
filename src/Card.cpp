#include "csv.h"
#include "Utils.h"
#include "Card.h"

std::vector<TripleTriad::Card,TripleTriad::Card::CardAllocator> TripleTriad::Card::_data_initialize() {
    std::vector<TripleTriad::Card,TripleTriad::Card::CardAllocator> out;
    io::CSVReader<6> data(CARD_DATA);
    std::string name;
    int score[4];
    char e;
    int j = 0;
    while (data.read_row(name, score[0], score[1], score[2], score[3], e)) {
        out.emplace_back(name, TripleTriad::elementMap.at(e), score);
        ++j;
    }
    return out;
}
std::unordered_map<std::string, TripleTriad::Card const*> TripleTriad::Card::_map_initialize() {
    std::unordered_map<std::string, TripleTriad::Card const*> map;
    for (int i = 0; i < 109; ++i) {
        map.insert({ TripleTriad::Card::_database[i].name(), &TripleTriad::Card::_database[i] });
    }
    return map;
}

std::vector<TripleTriad::Card,TripleTriad::Card::CardAllocator> TripleTriad::Card::_database
= TripleTriad::Card::_data_initialize();
std::unordered_map<std::string, TripleTriad::Card const*> TripleTriad::Card::_map
= TripleTriad::Card::_map_initialize();

TripleTriad::Card const& TripleTriad::Card::getCard(std::string const &name) { return *_map.at(name); }

TripleTriad::Card::Card(std::string card_name, TripleTriad::Element element, int *score) : _name(std::move(card_name)),
 _element(element) {
    memcpy(_score, score, 4 * sizeof(int));
}


