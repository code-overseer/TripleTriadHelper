#ifndef TRIPLETRIAD_CARD_H
#define TRIPLETRIAD_CARD_H

#include "Utils.h"

namespace TripleTriad {
    struct Card {
        struct CardAllocator : std::allocator<Card> {
            template< class U, class... Args >
            void construct( U* p, Args&&... args ) {
                ::new((void *)p) U(std::forward<Args>(args)...);
            }

            template< class U > struct rebind { typedef CardAllocator other; };
        };
    private:
        std::string _name;
        int _score[4] = {0, 0, 0, 0};
        Element _element = None;
        Card(std::string card_name, Element element, int score[4]);
        Card(Card const &other) = default;
        static std::vector<Card, CardAllocator> _database;
        static std::unordered_map<std::string, Card const*> _map;
        static std::unordered_map<std::string, Card const*> _map_initialize();
        static std::vector<Card,CardAllocator> _data_initialize();
    public:
        friend class CardAllocator;
        static Card const& getCard(std::string const &name);
        inline Element const &element() const { return _element; }
        inline std::string const& name() const { return _name; }
        inline int const* score() const { return _score; }
        inline int up() const { return _score[0]; }
        inline int down() const { return _score[1]; }
        inline int right() const { return _score[2]; }
        inline int left() const { return _score[3]; }
    };
}

#endif //TRIPLETRIAD_CARD_H
