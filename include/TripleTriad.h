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

enum Element { Fire, Ice, Water, Earth, Holy, Poison, Thunder, None };
enum Rule { Same, SameWall, Plus, Elemental };
enum Team { Blue, Red };
typedef std::unordered_map<Team, int> Score;
typedef std::initializer_list<Rule> Rules;
typedef std::initializer_list<Element> Elements;

#endif //TRIPLETRIAD_TRIPLETRIAD_H
