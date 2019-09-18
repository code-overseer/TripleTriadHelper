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

enum Element { Fire, Ice, Water, Earth, Holy, Poison, Thunder, Wind, None };
enum Rule { Same, SameWall, Plus, Elemental };
enum Team { Blue, Red };
typedef std::unordered_map<Team, int> Score;
typedef std::unordered_map<Rule, bool> Rules;
typedef std::vector<Element> Elements;
static std::unordered_map<char, Element> const elementMap = // NOLINT(cert-err58-cpp)
        {{'N', None}, {'T', Thunder}, {'E', Earth}, {'P', Poison}, {'F', Fire},
         {'I', Ice}, {'A', Water}, {'W', Wind}, {'H', Holy}};

#endif //TRIPLETRIAD_TRIPLETRIAD_H
