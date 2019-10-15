#include <Board.h>
#include <ncurses.h>
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
    Team turn = toupper(result["turn"].as<char>()) == 'R' ? Red : Blue;
    Board game(rules, rules.at(Elemental) ? result["elemental"].as<std::string>() : "", turn);
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
//                GUI::showHint(game.hint(blue, red));
                move(37, 0);
            } else if (strlen(input) < 4) {
                card_no = atoi(strtok(input, ":"));
                pos = atoi(strtok(nullptr, ":"));
                if (turn == Red) {
                    game.play(red[card_no].name(), pos);
                    red.erase(red.begin() + card_no);
                    turn = Blue;
                } else {
                    game.play(blue[card_no].name(), pos);
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
    Team turn = toupper(result["turn"].as<char>()) == 'R' ? Red : Blue;
    Board game(rules, rules.at(Elemental) ? result["elemental"].as<std::string>() : "", turn);
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
            if (game.turn() == Blue && !strcmp(input, "hint")) {
//                GUI::showHint(game.hint(blue));
                move(37, 0);
            } else if (game.turn() == Blue && strlen(input) < 4) {
                card_no = atoi(strtok(input, ":"));
                pos = atoi(strtok(nullptr, ":"));
                game.play(blue[card_no].name(), pos);
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
    std::vector<Card> blue = player_cards("Edea,Quistis,Bahamut,Odin,Leviathan", Blue);
    std::vector<Card> red = player_cards("Squall,Blitz,Elastoid,GIM47N,Adamantoise", Red);
    std::unordered_map<Rule, bool> rules = {{Same, true},
                                            {SameWall, true},
                                            {Plus, false},
                                            {Elemental, false}};
    Board game(rules, "", Red);
    game.play(red[0].name(), 0);
    game.play(blue[1].name(), 3);
    game.play(red[3].name(), 7);
    game.play(blue[4].name(), 8);
    game.play(red[4].name(), 4);
    std::cout<<game.score(Red)<<':'<<game.score(Blue)<<std::endl;
}

std::string TripleTriad::get_hint(Board const &main, std::vector<Card> const &player, std::vector<Card> const &enemy) {
    static auto cmp = [](const Node& left, const Node& right) { return left.score > right.score; };
    std::unordered_map<Team, std::vector<Card> const &> cards = {{Blue, player}, {Red, enemy}};
    Node previous("root", player, enemy, main);
    std::priority_queue<Node, std::vector<Node>, decltype(cmp)> branches(cmp);
    std::vector<Node> results;
    auto blanks = previous.board.getBlanks();
    for (auto const &blank : blanks) {
        int card_idx = 0;
        auto turn = previous.turn();
        for (auto const &card : cards.at(turn)) {
            if (!previous.used.at(turn)[card_idx])
                branches.push(previous.forward(card.name(), card_idx, blank->idx()));
            ++card_idx;
        }
    }

    while (!branches.empty()) {
        auto const &max = branches.top();
        if (max.score < -0.5 && max.turn() == Red) {
            branches.pop();
        } else if (!max.total.at(Blue) || !max.total.at(Red) || max.turns(Blue) >= 3
        || (max.score > 3 && max.turn() == Blue)) {
            results.emplace_back(max);
            branches.pop();
        } else {
            previous = max;
            blanks = previous.board.getBlanks();
            for (auto const &blank : blanks) {
                int card_idx = 0;
                auto turn = previous.turn();
                for (auto const &card : cards.at(turn)) {
                    if (!previous.used.at(turn)[card_idx])
                        branches.push(previous.forward(previous.name, card_idx, blank->idx()));
                    ++card_idx;
                }
            }
        }
    }
    std::sort(results.begin(), results.end(), cmp);
    std::stringstream out_stream;
    out_stream << std::left << std::setw(6) << std::setfill(' ') << "No.";
    out_stream << std::left << std::setw(15) << std::setfill(' ') << "Card";
    out_stream << std::left << std::setw(9) << std::setfill(' ') << "Position";
    out_stream << std::left << std::setw(15) << std::setfill(' ') << "Potential";
    out_stream << '\n';

    for (int i = 0; i < 9 && i < (int)results.size(); ++i) {
        out_stream << std::left << std::setw(6) << std::setfill(' ') << i + 1;
        out_stream << std::left << std::setw(15) << std::setfill(' ') << results[i].name;
        out_stream << std::left << std::setw(9) << std::setfill(' ') << results[i].position;
        out_stream << std::left << std::setw(15) << std::setfill(' ') << results[i].score;
        out_stream << '\n';
    }

    return out_stream.str();

}




