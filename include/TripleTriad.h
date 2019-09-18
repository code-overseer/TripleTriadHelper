#ifndef TRIPLETRIAD_TRIPLETRIAD_H
#define TRIPLETRIAD_TRIPLETRIAD_H

#include<iostream>
#include<cstdio>
#include<initializer_list>
#include<vector>
#include<unordered_map>
#include<functional>
#include<set>
#include<queue>
#include<stack>
#include<string>
#include<sstream>
#include<map>
#include<utility>
#include<iomanip>
#include "cxxopts.hpp"

namespace TripleTriad {
    class Card;
    enum Element { Fire, Ice, Water, Earth, Holy, Poison, Thunder, Wind, None };
    enum Rule { Same, SameWall, Plus, Elemental };
    enum Team { Blue, Red };
    typedef std::unordered_map<Team, int> Score;
    typedef std::unordered_map<Rule, bool> Rules;
    static std::unordered_map<char, Element> const elementMap = // NOLINT(cert-err58-cpp)
            {{'N', None}, {'T', Thunder}, {'E', Earth}, {'P', Poison}, {'F', Fire},
             {'I', Ice}, {'A', Water}, {'W', Wind}, {'H', Holy}};

    cxxopts::Options option_parser();
    std::vector<Card> player_cards(std::string const& names, Team team);
    void open_game(cxxopts::ParseResult const &result);
    void close_game(cxxopts::ParseResult const &result);
}


#endif //TRIPLETRIAD_TRIPLETRIAD_H
