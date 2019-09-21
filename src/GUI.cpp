//
// Created by Bryan Wong on 2019-09-20.
//

#include <GUI.h>
#include <ncurses.h>
#include "Board.h"
#include "Card.h"

void TripleTriad::GUI::init() {
    initscr();
    cbreak();
    noecho();

    clear();
}

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
    mvaddch(root.second + 1, root.first + 3, element_map.at(elemental));

    if (!card) return;
    char score = static_cast<char>('0' + card->n(true));
    mvaddch(root.second + 1, root.first + 2, score > '9' ? 'A' : score); // north
    score = static_cast<char>('0' + card->s(true));
    mvaddch(root.second + 3, root.first + 2, score > '9' ? 'A' : score); // south
    score = static_cast<char>('0' + card->e(true));
    mvaddch(root.second + 2, root.first + 3, score > '9' ? 'A' : score); // east
    score = static_cast<char>('0' + card->w(true));
    mvaddch(root.second + 2, root.first + 1, score > '9' ? 'A' : score); // west
}

void TripleTriad::GUI::drawPlayerList(const std::vector<Card> &cards, Team team) {
    int y = 1;
    if (team == Red) {
        for (int i = 0; i < 5; ++i) {
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
    int y = 1;
    for (int i = 0; i < 5; ++i) {
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
        if (c) {
            drawBlock({x, y}, c->element(), c);
            mvaddch(y + 2, x + 2, c->team() == Red ? 'R' : 'B');
        } else {
            drawBlock({x, y}, board.element(i));
            mvaddch(y + 2, x + 2, '0' + i);
        }
        if (board.isElemental(i)) mvaddch(y + 1, x + 3, toupper(element_map.at(c->element())));
    }
    char score_card[3];
    score_card[0] = static_cast<char>('0' + board.score(Red));
    score_card[1] = ':';
    score_card[2] = static_cast<char>('0' + board.score(Blue));
    if (score_card[0] > '9') score_card[0] = 'A';
    if (score_card[2] > '9') score_card[2] = 'A';
    mvaddstr(33,0, "Input: [Card no] [Pos no]\n");
    mvaddstr(34,0, "For Blue turn only, input: hint\n");
    mvaddstr(35,0, "For non-Open game, opponent input: [CardName] [Pos no]");
    mvaddnstr(24, 15, score_card, 3);
}

void TripleTriad::GUI::changeTurn(TripleTriad::Team team) {
    mvaddch(0, (team == Red ? 3 : 32), '*');
    mvaddch(0, (team == Red ? 32 : 3), ' ');
}

void TripleTriad::GUI::draw(const TripleTriad::Board &board, TripleTriad::Team turn, const std::vector<Card> &blue,
                            const std::vector<Card> &red) {
    clear();
    drawBoard(board);
    drawPlayerList(blue, Blue);
    drawPlayerList(red, Red);
    changeTurn(turn);
    refresh();
}

void TripleTriad::GUI::draw(const TripleTriad::Board &board, TripleTriad::Team turn, const std::vector<Card> &blue,
                            int unknowns) {
    clear();
    drawBoard(board);
    drawPlayerList(blue, Blue);
    drawPlayerList(unknowns);
    changeTurn(turn);
    refresh();
}

void TripleTriad::GUI::showHint(std::string const &hint) {
    char hint_c[hint.size() + 1];
    strcpy(hint_c, hint.c_str());
    char* line = strtok(hint_c, "\n");

    for (int i = 0; i < 9; ++i, line = strtok(nullptr, "\n")) {
        move(27 + i, 0);
        clrtoeol();
        mvaddstr(27 + i, 0, line);
    }
    move(37, 0);
    clrtoeol();
    getch();
}

void TripleTriad::GUI::invalid_input(char const* input) {
    for (int i = 27; i < 36; ++i) {
        move(i, 0);
        clrtoeol();
    }
    char msg[80];
    sprintf(msg, "ERROR: \"%s\" is an invalid input!", input);
    mvaddnstr(27, 0, msg, 80);
    move(37, 0);
    clrtoeol();
    getch();
}

