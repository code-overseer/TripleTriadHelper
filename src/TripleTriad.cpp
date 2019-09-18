#include <Board.h>
#include "TripleTriad.h"
#include "Card.h"

cxxopts::Options TripleTriad::option_parser() {
    using namespace cxxopts;
    static Options options("TTHelper", "Find current move for Triple Triad in FFVIII");
    static bool init = false;
    if (init) return options;
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
    return options;
}

std::vector<TripleTriad::Card> TripleTriad::player_cards(std::string const &names, Team team) {
    std::vector<Card> cards;
    std::istringstream stream(names);
    std::string name;
    while (std::getline(stream, name, ',')) {
        cards.emplace_back( Card(name.c_str(), team) );
    }
    if (cards.size() != 5) throw std::runtime_error("Must have 5 cards");
    return cards;
}

void TripleTriad::open_game(cxxopts::ParseResult const &result) {
    std::vector<Card> blue = player_cards(result["blue"].as<std::string>(), Blue);
    std::vector<Card> red = player_cards(result["red"].as<std::string>(), Red);
    std::unordered_map<Rule, bool> rules = {{Same, result["same"].as<bool>()},
                                            {SameWall, result["wall"].as<bool>()},
                                            {Plus, result["plus"].as<bool>()},
                                            {Elemental, result["elemental"].count()}};
    Board game(rules, rules.at(Elemental) ? result["elemental"].as<std::string>() : "");
    Team turn = toupper(result["turn"].as<char>()) == 'R' ? Red : Blue;


}

void TripleTriad::close_game(cxxopts::ParseResult const &result) {
    std::vector<Card> blue = player_cards(result["blue"].as<std::string>(), Blue);
    std::unordered_map<Rule, bool> rules = {{Same, result["same"].as<bool>()},
                                            {SameWall, result["wall"].as<bool>()},
                                            {Plus, result["plus"].as<bool>()}};
    auto red = Card::cardFinder();
    Board game(rules, rules.at(Elemental) ? result["elemental"].as<std::string>() : "");

}



