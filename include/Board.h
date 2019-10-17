#ifndef TRIPLETRIAD_BOARD_H
#define TRIPLETRIAD_BOARD_H

#include "Utils.h"
#include "Position.h"
#include "GUI.h"

namespace TripleTriad {
    class Board {
        struct flips_t {
            std::string card = "";
            int pos = -1;
            std::set<Position*> flips = std::set<Position *>();
            flips_t() = default;
        } _flips;
        Rules _rules;
        Team _turn = Blue;
        Score _score = { {Red, 5}, {Blue, 5} };
        std::vector<Position, Position::PositionAllocator> _pos;
        std::vector<std::list<Position*>> _adj;
        void _getFlips(std::string const &card_name, int pos);
        void _getDefaultFlips(std::set<Position *> &flips, int pos) const;
        void _getComboFlips(std::set<Position *> &flips, int pos, bool visited[9]) const;
        void _getSameFlips(std::set<Position *> &flips, int pos) const;
        void _getPlusFlips(std::set<Position *> &flips, int pos) const;
        void _adjacent();
    public:
        Board() = default;
        Board(Rules &&rules, std::string const &elements, Team turn);
        Board(Board const &other);
        Board(Board &&other) noexcept;
        Board &operator=(Board const &other);
        Board &operator=(Board &&other) noexcept;
        inline int const &score(Team team) const { return _score.at(team); }
        inline Team const &turn() const { return _turn; }
        inline Element const &element(int i) { return _pos[i].element(); }
        inline Team const &team(int i) const { return _pos[i].team(); }
        int play(std::string const &card, int pos);
        int calculate(std::string const &card, int pos);
        std::list<Position const*> getBlanks() const;
        friend void GUI::drawBoard(const TripleTriad::Board &board);
    };
}

#endif //TRIPLETRIAD_BOARD_H
