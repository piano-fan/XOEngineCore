#ifndef XO_SQUARETRACKER_H
#define XO_SQUARETRACKER_H

#include "types.h"


namespace XO{
    using SquareTracker_Container = std::list<Point>;
    template<ValueT TIER_COUNT>
    class SquareTracker{
        using Lists = std::array<SquareTracker_Container, TIER_COUNT>;
        using Hooks = std::array<SquareTracker_Container::iterator, TIER_COUNT>;
        std::array<Lists, 2> m_lists;
        std::vector<std::array<Hooks, 2>> m_hooks;

        SquareTracker_Container& GetRef(Piece owner, ValueT tier){
            return m_lists[owner][tier];
        }

        SquareTracker_Container::iterator& GetHookRef(const Move &m, ValueT tier){
            return m_hooks[m.GetPos().GetID()][m.GetTurn()][tier];
        }

    public:
        void Alloc(DValueT sq_count){
            m_hooks.resize(sq_count);
        }

        void Clear(){
            m_lists.fill(Lists());
        }

        constexpr ValueT TierCount() const{
            return TIER_COUNT;
        }

        const SquareTracker_Container& Get(Piece owner, ValueT tier) const{
            return m_lists[owner][tier];
        }

        void Update(const Move& m, TacticSet before, TacticSet after){
            if(before == after){
                return;
            }
            auto promotions = after.Promotions(before);
            auto demotions = before.Promotions(after);
            for(ValueT tier = 0; tier < TIER_COUNT; ++tier){
                auto square_property = TProperty(tier);
                if(promotions.GetProperty(square_property)){
                    Init(m, tier);
                }
                if(demotions.GetProperty(square_property)){
                    Delete(m, tier);
                }
            }
        }

        void Init(const Move& m, ValueT tier){
            GetRef(m.GetTurn(), tier).push_back(m.GetPos());
            GetHookRef(m, tier) = --GetRef(m.GetTurn(), tier).end();
        }

        void Delete(const Move& m, ValueT tier){
            GetRef(m.GetTurn(), tier).erase(GetHookRef(m, tier));
        }

        std::string ToString(Piece p) const{
            std::string result;
            for(ValueT tier = 0; tier < TIER_COUNT; ++tier){
                auto square_property = TProperty(tier);
                result += "Tier " + std::to_string(tier) + ": ";
                for (auto &sq: Get(p, tier)) {
                    result += sq.ToString() + " ";
                }
                result += "\t";
            }
            return result;
        }
    };
}

#endif //XO_SQUARETRACKER_H
