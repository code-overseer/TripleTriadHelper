#include "csv.h"
#include "Card.h"

#include <utility>
#include "Position.h"

TripleTriad::Card const *TripleTriad::Card::cardList(int i) {
    static std::vector<Card const> database(static_cast<size_t>(109));
    if (database.empty()) {
        io::CSVReader<6> data(CARD_DATA);
        std::string name;
        int score[4];
        char e;
        int j = 0;
        while (data.read_row(name, score[0], score[1], score[2], score[3], e)) {
            database.emplace_back(name, elementMap.at(e), score);
            ++j;
        }
    }
    return &database[i];
}

TripleTriad::Card const& TripleTriad::Card::getCard(std::string const &name) {
    static std::unordered_map<std::string, Card const*> map;
    if (map.empty()) {
        for (int i = 0; i < 109; ++i) {
            auto c = cardList(i);
            map.insert({ c->name(), c });
        }
    }
    return *map.at(name);
}

TripleTriad::Card::Card(std::string card_name, TripleTriad::Element element, int *score) : _name(std::move(card_name)),
 _element(element) {
    memcpy(_score, score, 4 * sizeof(int));
}
