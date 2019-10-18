#ifndef TRIPLETRIAD_BRANCH_H
#define TRIPLETRIAD_BRANCH_H

#include "Utils.h"

namespace TripleTriad {
    struct Board;
    struct Branch {
    private:
        struct Node;
        static float alphabeta(Node const &node, int depth = 4, float alpha = -50, float beta = 50);
    public:
        int pos = -1;
        int card = -1;
        float val = 0;
        static bool compare(const Branch& left, const Branch& right) { return left.val > right.val; };
        Branch() = default;
        Branch(int first_card, int pos, std::vector<std::string> const &player,
               std::vector<std::string> const &enemy, Board const &main);
    };
}

#endif //TRIPLETRIAD_BRANCH_H
