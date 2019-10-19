#ifndef TRIPLETRIAD_GUI_H
#define TRIPLETRIAD_GUI_H

#include <TripleTriad.h>

namespace TripleTriad {
    class Board;
    class Card;
    class Position;
    namespace GUI {
        static std::unordered_map<Element, char const> const element_map = // NOLINT(cert-err58-cpp)
                {{None, ' '}, {Thunder, 'T'}, {Earth, 'E'}, {Poison, 'P'}, {Fire, 'F'},
                 {Ice, 'I'}, {Water, 'A'}, {Wind, 'W'}, {Holy, 'H'}};
        void init();
        void drawBlock(std::pair<int, int> const &root);
        void drawBlock(std::pair<int, int> const &root, Card const &card);
        void drawBlock(std::pair<int, int> const &root, Position const &pos);
        void clearBlock(std::pair<int, int> const &root);
        void drawPlayerList(std::vector<std::string> const &cards, Team team);
        void drawPlayerList(int unknown_count);
        void drawBoard(Board const &board);
        void changeTurn(Team team);
        void invalid_input(char const* input);
        void showHint(std::string const& hint);
        void draw(Board const &board, std::vector<std::string> const &blue, std::vector<std::string> const &red);
        void draw(Board const &board, std::vector<std::string> const &blue, int unknowns);
        void endgame(int blue_score);
    };
}

#endif //TRIPLETRIAD_GUI_H
