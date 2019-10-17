#ifndef TRIPLETRIAD_NODE_H
#define TRIPLETRIAD_NODE_H

#include "Utils.h"
#include "Board.h"
namespace TripleTriad {
    struct Node {
    private:
        struct stat_t {
            std::vector<std::string> const *cards = nullptr;
            std::vector<bool> isUsed;
            float delta = 0;
            inline int remaining() const { return static_cast<int>(cards->size() - used()); };
            inline int used() const { return std::accumulate(isUsed.begin(), isUsed.end(), 0);}
            inline int turns() const { return static_cast<int>(isUsed.size()); }
            inline int size() const { return static_cast<int>(cards->size()); }
            stat_t(stat_t const &other) = default;
            stat_t(stat_t &&other) noexcept = default;
            stat_t &operator=(stat_t const &other) = default;
            stat_t &operator=(stat_t &&other) = default;
            inline explicit stat_t(std::vector<std::string> const &list) : cards(&list),
            isUsed(std::vector<bool>(list.size())) {
                for (int i = 0; i < cards->size(); ++i) isUsed.emplace_back(false);
            }
        };
        int _uid = -1;
        int _position = -1;
        float _value = 0;
        Board _board;
        std::unordered_map<Team, stat_t> _stats;
        float _heuristic(int gain);
    public:
        Node(Node const &other);
        Node(Node &&other) noexcept;
        Node &operator=(Node const &other);
        Node &operator=(Node &&other) noexcept;
        Node(int first_card, int pos, std::vector<std::string> const &player,
                std::vector<std::string> const &enemy, Board main);
        Node forward(int card_idx, int pos) const;
        inline Node forward() const { return forward(_uid, _position); }
        inline int const &uid() const { return _uid; }
        inline int const &position() const { return _position; }
        inline int size(Team t) const { return _stats.at(t).size(); }
        inline int total(Team t) const { return _stats.at(t).remaining(); }
        inline int turns(Team t) const { return _stats.at(t).turns(); }
        inline bool used(Team t, int i) const { return _stats.at(t).isUsed[i]; }
        inline std::list<Position const *> blanks() const { return _board.getBlanks(); }
        inline std::vector<std::string> const &cards(Team t) const { return *_stats.at(t).cards;}
        inline float const &delta(Team t) const { return _stats.at(t).delta; }
        inline Team const &turn() const { return _board.turn(); }
        inline float const &value() const { return _value; }
        inline Board const &board() const { return _board; }
        inline bool terminated() const  { return _board.getBlanks().empty(); }
    };
}
#endif //TRIPLETRIAD_NODE_H
