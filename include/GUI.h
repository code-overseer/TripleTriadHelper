//
// Created by Bryan Wong on 2019-09-20.
//

#ifndef TRIPLETRIAD_GUI_H
#define TRIPLETRIAD_GUI_H

#include <TripleTriad.h>

namespace TripleTriad {
    class Board;
    class Card;
    namespace GUI {
        static std::unordered_map<Element, char const> const element_map = // NOLINT(cert-err58-cpp)
                {{None, ' '}, {Thunder, 't'}, {Earth, 'e'}, {Poison, 'p'}, {Fire, 'f'},
                 {Ice, 'i'}, {Water, 'a'}, {Wind, 'w'}, {Holy, 'h'}};
        void drawBlock(std::pair<int, int> const &root, Element elemental = None, Card const* card = nullptr);
        void clearBlock(std::pair<int, int> const &root);
        void drawPlayerList(std::vector<Card> const &cards, Team team);
        void drawPlayerList(int unknown_count);
        void drawBoard(Board const &board);
        void changeTurn(Team team);
    };
}

#endif //TRIPLETRIAD_GUI_H
