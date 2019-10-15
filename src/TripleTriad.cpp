#include <Board.h>
#include <ncurses.h>
#include "TripleTriad.h"
#include "Card.h"
#include "GUI.h"

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
    if (blue.size() != 5 && blue.size() != red.size())
        throw std::runtime_error("Not enough cards supplied, each team must have 5 cards");
    std::unordered_map<Rule, bool> rules = {{Same, result["same"].as<bool>()},
                                            {SameWall, result["wall"].as<bool>()},
                                            {Plus, result["plus"].as<bool>()},
                                            {Elemental, result["elemental"].count()}};
    Board game(rules, rules.at(Elemental) ? result["elemental"].as<std::string>() : "");
    Team turn = toupper(result["turn"].as<char>()) == 'R' ? Red : Blue;
    GUI::init();
    int plays = 0;
    char input[64];
    int card_no;
    int pos;
    while (plays < 9) {
        GUI::draw(game, turn, blue, red);
        move(37, 0);
        echo();
        getnstr(input, 5);
        if (!strlen(input)) continue;
        noecho();
        move(37, 0);
        clrtoeol();
        try {
            if (turn == Blue && !strcmp(input, "hint")) {
                GUI::showHint(game.hint(blue, red));
                move(37, 0);
            } else if (strlen(input) < 4) {
                card_no = atoi(strtok(input, ":"));
                pos = atoi(strtok(nullptr, ":"));
                if (turn == Red) {
                    game.play(red[card_no], pos);
                    red.erase(red.begin() + card_no);
                    turn = Blue;
                } else {
                    game.play(blue[card_no], pos);
                    blue.erase(blue.begin() + card_no);
                    turn = Red;
                }
                ++plays;
            } else {
                throw std::exception();
            }
        } catch (std::exception &e) {
            GUI::invalid_input(input);
            continue;
        }
    }
    endwin();
}

void TripleTriad::close_game(cxxopts::ParseResult const &result) {
    std::vector<Card> blue = player_cards(result["blue"].as<std::string>(), Blue);

    if (blue.size() != 5) throw std::runtime_error("Not enough cards supplied, each team must have 5 cards");

    std::unordered_map<Rule, bool> rules = {{Same, result["same"].as<bool>()},
                                            {SameWall, result["wall"].as<bool>()},
                                            {Plus, result["plus"].as<bool>()},
                                            {Elemental, result["elemental"].count()}};
    Board game(rules, rules.at(Elemental) ? result["elemental"].as<std::string>() : "");
    Team turn = toupper(result["turn"].as<char>()) == 'R' ? Red : Blue;
    GUI::init();
    int plays = 0;
    int red_cards = 5;
    char input[64];

    int card_no;
    int pos;
    while (plays < 9) {
        GUI::draw(game, turn, blue, red_cards);
        move(37, 0);
        echo();
        getnstr(input, 32);
        if (!strlen(input)) continue;
        noecho();
        move(37, 0);
        clrtoeol();
        try {
            if (turn == Blue && !strcmp(input, "hint")) {
                GUI::showHint(game.hint(blue));
                move(37, 0);
            } else if (turn == Blue && strlen(input) < 4) {
                card_no = atoi(strtok(input, ":"));
                pos = atoi(strtok(nullptr, ":"));
                game.play(blue[card_no], pos);
                ++plays;
                blue.erase(blue.begin() + card_no);
                turn = Red;
            } else if (turn == Red) {
                auto c = Card(strtok(input, ":"), Red);
                game.play(c, atoi(strtok(nullptr, ":")));
                ++plays;
                --red_cards;
                turn = Blue;
            } else {
                throw std::exception();
            }
        } catch (std::exception &e) {
            GUI::invalid_input(input);
            continue;
        }
    }
    endwin();


}

void TripleTriad::test_open() {
    std::vector<Card> blue = player_cards("Edea,Quistis,Bahamut,Odin,Leviathan", Blue);
    std::vector<Card> red = player_cards("Squall,Blitz,Elastoid,GIM47N,Adamantoise", Red);
    std::unordered_map<Rule, bool> rules = {{Same, true},
                                            {SameWall, true},
                                            {Plus, false},
                                            {Elemental, false}};
    Board game(rules, "");
    game.play(red[0], 0);
    game.play(blue[1], 3);
    game.play(red[3], 7);
    game.play(blue[4], 8);
    game.play(red[4], 4);
    std::cout<<game.score(Red)<<':'<<game.score(Blue)<<std::endl;
}

std::string TripleTriad::get_hint(Board const &main, std::vector<Card> const &player, std::vector<Card> const &enemy) {
    static struct comparer_t {
        bool operator() (hint_t const &i, hint_t const &j) { return (i.score > j.score);}
    } compare;
    bool used_p[player.size()];
    bool used_e[enemy.size()];
    memset(used_p, 0, player.size());
    memset(used_e, 0, enemy.size());


}




