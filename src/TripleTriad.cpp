#include <ncurses.h>
#include "Board.h"
#include "TripleTriad.h"
#include "Card.h"
#include "GUI.h"
#include "Node.h"

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

std::vector<std::string> TripleTriad::player_cards(std::string const &names) {
    std::vector<std::string> cards;
    std::istringstream stream(names);
    std::string name;
    while (std::getline(stream, name, ',')) {
        cards.emplace_back( name );
    }
    if (cards.size() != 5) throw std::runtime_error("Must have 5 cards");
    return cards;
}

void TripleTriad::open_game(cxxopts::ParseResult const &result) {
    std::vector<std::string> blue = player_cards(result["blue"].as<std::string>());
    std::vector<std::string> red = player_cards(result["red"].as<std::string>());
    if (blue.size() != 5 && blue.size() != red.size())
        throw std::runtime_error("Not enough cards supplied, each team must have 5 cards");
    std::unordered_map<Rule, bool> rules = {{Same, result["same"].as<bool>()},
                                            {SameWall, result["wall"].as<bool>()},
                                            {Plus, result["plus"].as<bool>()},
                                            {Elemental, result["elemental"].count()}};
    Team turn = toupper(result["turn"].as<char>()) == 'R' ? Red : Blue;
    Board game(std::move(rules), rules.at(Elemental) ? result["elemental"].as<std::string>() : "", turn);
    GUI::init();
    int plays = 0;
    char input[64];
    int card_no;
    int pos;
    while (plays < 9) {
        GUI::draw(game, blue, red);
        move(37, 0);
        echo();
        getnstr(input, 5);
        if (!strlen(input)) continue;
        noecho();
        move(37, 0);
        clrtoeol();
        try {
            if (game.turn() == Blue && !strcmp(input, "hint")) {
                GUI::showHint(get_hint(game, blue, red));
                move(37, 0);
            } else if (strlen(input) < 4) {
                card_no = atoi(strtok(input, ":"));
                pos = atoi(strtok(nullptr, ":"));
                if (game.turn() == Red) {
                    game.play(red[card_no], pos);
                    red.erase(red.begin() + card_no);
                } else {
                    game.play(blue[card_no], pos);
                    blue.erase(blue.begin() + card_no);
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
    std::vector<std::string> blue = player_cards(result["blue"].as<std::string>());

    if (blue.size() != 5) throw std::runtime_error("Not enough cards supplied, each team must have 5 cards");

    std::unordered_map<Rule, bool> rules = {{Same, result["same"].as<bool>()},
                                            {SameWall, result["wall"].as<bool>()},
                                            {Plus, result["plus"].as<bool>()},
                                            {Elemental, result["elemental"].count()}};
    Team turn = toupper(result["turn"].as<char>()) == 'R' ? Red : Blue;
    Board game(std::move(rules), rules.at(Elemental) ? result["elemental"].as<std::string>() : "", turn);
    GUI::init();
    int plays = 0;
    int red_cards = 5;
    char input[64];

    int card_no;
    int pos;
    while (plays < 9) {
        GUI::draw(game, blue, red_cards);
        move(37, 0);
        echo();
        getnstr(input, 32);
        if (!strlen(input)) continue;
        noecho();
        move(37, 0);
        clrtoeol();
        try {
            if (game.turn() == Blue && !strcmp(input, "hint")) {
//                GUI::showHint(get_hint(game, blue, red));
                move(37, 0);
            } else if (game.turn() == Blue && strlen(input) < 4) {
                card_no = atoi(strtok(input, ":"));
                pos = atoi(strtok(nullptr, ":"));
                game.play(blue[card_no], pos);
                ++plays;
                blue.erase(blue.begin() + card_no);
            } else if (game.turn() == Red) {
                game.play(strtok(input, ":"), atoi(strtok(nullptr, ":")));
                ++plays;
                --red_cards;
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
    auto blue = player_cards("Edea,Quistis,Bahamut,Odin,Leviathan");
    auto red = player_cards("Squall,Blitz,Elastoid,GIM47N,Adamantoise");
    std::unordered_map<Rule, bool> rules = {{Same, true},
                                            {SameWall, true},
                                            {Plus, false},
                                            {Elemental, false}};
    Board game(std::move(rules), "", Red);
    game.play(red[0], 0);
    game.play(blue[1], 3);
    game.play(red[3], 7);
    game.play(blue[4], 8);
    game.play(red[4], 4);
    std::cout << game.score(Red)<<':' << game.score(Blue) << std::endl;
}


std::string TripleTriad::get_hint(Board const &main, std::vector<std::string> const &player,
        std::vector<std::string> const &enemy) {

    std::unordered_map<Team, std::vector<std::string> const &> cards = {{Blue, player}, {Red, enemy}};
    auto blanks = main.getBlanks();
    auto total = static_cast<int>(cards.at(Blue).size());
    std::vector<Node> branches;
    for (auto const &blank : blanks) {
        for (int i = 0; i < total; ++i) {
            branches.emplace_back(Node(i, blank->idx(), cards.at(Blue), cards.at(Red), main).forward());
        }
    }
    std::vector<score_t> scores;
    scores.reserve(branches.size());

    for (auto &branch : branches) {
        scores.emplace_back(branch.position(), branch.uid(), alphaBeta(branch));
    }

    std::sort(scores.begin(), scores.end(), score_t::compare);

    std::stringstream out_stream;
    out_stream << std::left << std::setw(6) << std::setfill(' ') << "No.";
    out_stream << std::left << std::setw(15) << std::setfill(' ') << "Card";
    out_stream << std::left << std::setw(9) << std::setfill(' ') << "Position";
    out_stream << std::left << std::setw(15) << std::setfill(' ') << "Potential";
    out_stream << '\n';

    for (int i = 0; i < 16 && i < (int)scores.size(); ++i) {
        out_stream << std::left << std::setw(6) << std::setfill(' ') << i + 1;
        out_stream << std::left << std::setw(15) << std::setfill(' ') << player[scores[i].card];
        out_stream << std::left << std::setw(9) << std::setfill(' ') << scores[i].pos;
        out_stream << std::left << std::setw(15) << std::setfill(' ') << scores[i].val;
        out_stream << '\n';
    }

    return out_stream.str();

}

void TripleTriad::test_hint() {
//    std::vector<Card> blue = player_cards("Edea,Quistis,Bahamut,Odin,Leviathan", Blue);
//    std::vector<Card> red = player_cards("Squall,Blitz,Elastoid,GIM47N,Adamantoise", Red);
    auto blue = player_cards("Edea,Quistis,Bahamut,Diablos,Leviathan");
    auto red = player_cards("Tri-Point,Wendigo,Gargantua,Bomb,Iron Giant");
    std::unordered_map<Rule, bool> rules = {{Same, true},
                                            {SameWall, true},
                                            {Plus, false},
                                            {Elemental, false}};

    Board game(std::move(rules), "", Blue);
//    game.play(blue[0], 6);
//    blue.erase(blue.begin());
//    game.play(red[0], 2);
//    red.erase(red.begin());
    std::cout << get_hint(game, blue, red) << std::endl;

}

float TripleTriad::alphaBeta(const TripleTriad::Node &node, int depth, float alpha, float beta) {

    if (!depth || node.value() < -5 || node.terminated()) return node.value();
    float val = -1e5;
    auto blanks = node.blanks();
    auto cards = static_cast<int>(node.cards(Blue).size());
    for (auto const &blank : blanks) {
        for (int i = 0; i < cards; ++i) {
            if (node.used(node.turn(), i)) continue;
            val = std::max(val, alphaBeta(node.forward(i, blank->idx()), depth - 1, alpha, beta));
            if (node.turn() == Blue) {
                alpha = std::max(alpha, val);
            } else {
                beta = std::max(beta, val);
            }
            if (alpha >= -beta) break;
        }
    }
    return val;
}




