//
// Created by Bryan Wong on 2019-09-20.
//

#include <GUI.h>
#include <ncurses.h>
#include "Board.h"
#include "Card.h"

void TripleTriad::GUI::drawBlock(std::pair<int, int> const &root, Element elemental, Card const *card) {
    mvaddch(root.second, root.first, '+');
    mvaddch(root.second + 4, root.first, '+');
    mvaddch(root.second, root.first + 4, '+');
    mvaddch(root.second + 4, root.first + 4, '+');

    for (int i = 1; i < 4; ++i) {
        mvaddch(root.second, root.first + i, '-'); // top
        mvaddch(root.second + 4, root.first + i, '-'); // bottom
        mvaddch(root.second + i, root.first, '|'); // left
        mvaddch(root.second + i, root.first + 4, '|'); // right
    }
    mvaddch(root.second + 2, root.first + 3, element_map.at(elemental));

    if (!card) return;

    mvaddch(root.second + 1, root.first + 2, '0' + card->n(true)); // north
    mvaddch(root.second + 3, root.first + 2, '0' + card->s(true)); // south
    mvaddch(root.second + 2, root.first + 1, '0' + card->e(true)); // east
    mvaddch(root.second + 2, root.first + 3, '0' + card->w(true)); // west
}

void TripleTriad::GUI::drawPlayerList(const std::vector<Card> &cards, Team team) {
    if (team == Red) {
        for (int i = 0; i < 5; ++i) {
            static int y = 1;
            if (i < cards.size()) {
                drawBlock({0, y}, cards[i].element(), &cards[i]);
                mvaddstr(27 + i, 0, "                  ");
                mvaddch(27 + i, 0, '0' + i);
                mvaddstr(27 + i, 1, ". ");
                mvaddstr(27 + i, 3, cards[i].name().c_str());
            } else {
                clearBlock({0, y});
                mvaddstr(27 + i, 0, "                  ");
            }
            y += 5;
        }
        return;
    }
    for (int i = 0; i < 5; ++i) {
        static int y = 1;
        if (i < cards.size()) {
            drawBlock({28, y}, cards[i].element(), &cards[i]);
            mvaddstr(27 + i, 28, "                  ");
            mvaddch(27 + i, 28, '0' + i);
            mvaddstr(27 + i, 29, ". ");
            mvaddstr(27 + i, 30, cards[i].name().c_str());
        } else {
            clearBlock({28, y});
            mvaddstr(27 + i, 28, "                  ");
        }
        y += 5;
    }
}

void TripleTriad::GUI::clearBlock(std::pair<int, int> const &root) {
    for (int i = 0; i < 5; ++i) {
        mvaddstr(root.second + i, root.first, "     ");
    }
}

void TripleTriad::GUI::drawPlayerList(int unknown_count) {
    for (int i = 0; i < 5; ++i) {
        static int y = 1;
        if (i < unknown_count) {
            drawBlock({0, y});
            mvaddch(27 + i, 0, '0' + i);
            mvaddstr(27 + i, 1, ". ");
            mvaddstr(27 + i, 3, "?????");
        } else {
            clearBlock({0, y});
            mvaddstr(27 + i, 0, "                  ");
        }
        y += 5;
    }
}

void TripleTriad::GUI::drawBoard(const TripleTriad::Board &board) {
    int y0 = 6, x0 = 9;
    mvaddstr(0,0,"RED");
    mvaddstr(0,28,"BLUE");
    for (int i = 0; i < 9; ++i) {
        int y = y0 + 5 * (i / 3);
        int x = x0 + 5 * (i % 3);
        auto c = board.card(i);
        if (c) drawBlock({x, y}, c->element(), c);
        else {
            drawBlock({x, y}, board.element(i));
            mvaddch(y + 2, x + 2, '0' + i);
        }
        if (board.isElemental(i)) mvaddch(y + 2, x + 3, toupper(element_map.at(c->element())));
    }
}

void TripleTriad::GUI::changeTurn(TripleTriad::Team team) {
    mvaddch(0, (team == Red ? 3 : 32), '*');
}

