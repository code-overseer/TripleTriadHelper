#include "Board.h"
#include "TripleTriad.h"
#include "cxxopts.hpp"

int main(int argc, char* argv[]) {
    using namespace TripleTriad;
    using namespace cxxopts;
    Options options("TTHelper", "Find current move for Triple Triad in FFVIII");
    options.add_option("Rules", "s","same", "Activate the Same Rule", value<bool>(), "");
    options.add_option("Rules", "w","wall", "Activate the SameWall Rule", value<bool>(), "");
    options.add_option("Rules", "p","plus", "Activate the Plus Rule", value<bool>(), "");
    options.add_option("Rules", "e","elemental", "Activate the Plus Rule", value<std::string>(), "A: Water\nF: Fire\n"
                                                                                                 "T: Thunder\nE: Earth\n"
                                                                                                 "P: Poison\nH: Holy\n"
                                                                                                 "I: Ice\nN:None\n"
                                                                                                 "W: Wind\n");
    options.add_option("Card", "b","blue", "Player", value<std::string>(), "Type in card names separated by ',' "
                                                                           "only (See csv file for list of cards)");
    options.add_option("Card", "r","red", "Opponent", value<std::string>(), "Type in card names separated by ',' "
                                                                            "only (See csv file for list of cards)");
    options.add_option("Turn", "t","turn", "First turn", value<char>(), "R: red\nB: blue");

    auto result = options.parse(argc, argv);
    std::vector<Card> red;
    std::vector<Card> blue;
    std::istringstream stream(result["blue"].as<std::string>());
    std::string token;
    while (std::getline(stream, token, ',')) {
        blue.emplace_back( Card::Factory(token.c_str(), Blue) );
    }
    if (blue.size() != 5) throw std::runtime_error("Must have 5 cards");
    stream.clear();
    bool isOpen = result["red"].count();
    if (isOpen) {
        stream = std::istringstream(result["red"].as<std::string>());
        while (std::getline(stream, token, ',')) {
            red.emplace_back( Card::Factory(token.c_str(), Red) );
        }
    }
    std::multimap<std::string, Card*> blue_cards;
    std::multimap<std::string, Card*> red_cards;

    std::unordered_map<Rule, bool> rules = {{Same, result["same"].as<bool>()},
                                            {SameWall, result["wall"].as<bool>()},
                                            {Plus, result["plus"].as<bool>()}};
    std::string elements = result["elemental"].count() ? result["elemental"].as<std::string>() : "";
    Board game(rules, elements);




    return 0;
}