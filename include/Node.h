#ifndef TRIPLETRIAD_NODE_H
#define TRIPLETRIAD_NODE_H

#include "Board.h"
#include "Branch.h"

namespace TripleTriad {
    struct Branch::Node {
    public:
        struct stat_t {
            std::vector<std::string> const *cards = nullptr;
            std::vector<bool> isUsed;
            float delta = 0;
            inline int remaining() const { return static_cast<int>(cards->size() - used()); };
            inline int used() const { return std::accumulate(isUsed.begin(), isUsed.end(), 0);}
            inline int size() const { return static_cast<int>(cards->size()); }
            stat_t(stat_t const &other) = default;
            stat_t(stat_t &&other) noexcept = default;
            stat_t &operator=(stat_t const &other) = default;
            stat_t &operator=(stat_t &&other) = default;
            inline explicit stat_t(std::vector<std::string> const &list) : cards(&list), isUsed() {
                isUsed.reserve(list.size());
                auto n = static_cast<int>(cards->size());
                for (int i = 0; i < n; ++i) isUsed.emplace_back(false);
            }
        };
        int uid = -1;
        int position = -1;
        float value = 0;
        Board board;
        std::unordered_map<Team, stat_t> stats;
        float heuristic(int gain);
        Node(Node const &other);
        Node(Node &&other) noexcept;
        Node &operator=(Node const &other);
        Node &operator=(Node &&other) noexcept;
        Node(int first_card, int pos, std::vector<std::string> const &player,
                std::vector<std::string> const &enemy, Board main);
        Node forward(int card_idx, int blank) const;
        inline std::list<Position const*> blanks() const  { return board.getBlanks(); }
        inline bool terminated() const  { return blanks().empty(); }
        inline bool used(Team t, int i) const { return stats.at(t).isUsed[i]; }
        inline Team const &turn() const { return board.turn(); }
        inline std::vector<std::string> const &cards(Team t) const { return *stats.at(t).cards; }
    };
}
#endif //TRIPLETRIAD_NODE_H
