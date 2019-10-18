#ifndef TRIPLETRIAD_UTILS_H
#define TRIPLETRIAD_UTILS_H
#include <iostream>
#include <cstdio>
#include <initializer_list>
#include <vector>
#include <unordered_map>
#include <functional>
#include <set>
#include <queue>
#include <stack>
#include <string>
#include <sstream>
#include <thread>
#include <map>
#include <utility>
#include <numeric>
#include <iomanip>
#include <list>
#include <ncurses.h>
#include "cxxopts.hpp"

namespace TripleTriad {
    class Card;
    enum Element { Fire, Ice, Water, Earth, Holy, Poison, Thunder, Wind, None };
    enum Rule { Same, SameWall, Plus, Elemental };
    enum Team { Blue, Red };
    enum Direction { Up, Down, Right, Left };
    typedef std::unordered_map<Team, int> Score;
    typedef std::unordered_map<Rule, bool> Rules;
    static std::unordered_map<char, Element> const elementMap = // NOLINT(cert-err58-cpp)
            {{'N', None}, {'T', Thunder}, {'E', Earth}, {'P', Poison}, {'F', Fire},
             {'I', Ice}, {'A', Water}, {'W', Wind}, {'H', Holy}};
}

#endif //TRIPLETRIAD_UTILS_H
