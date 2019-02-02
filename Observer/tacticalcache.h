#ifndef XO_TACTICALCACHE_H
#define XO_TACTICALCACHE_H

#include <memory>
#include <array>
#include "tacticset.h"


namespace XO{
    class TacticalCache{
        static const unsigned int TOTAL_TACTICS_COUNT = 256;
        std::array<TacticSet, TOTAL_TACTICS_COUNT> m_data;
        static TacticalCache m_instance;

        constexpr static ValueT ThreatPairToID(Threat primary, Threat secondary){
            return (primary.GetFullTier() << 4) + secondary.GetFullTier();
        }
        static ValueT ThreatSetToID(const SortedThreatSet& threats){
            return ThreatPairToID(threats[0], threats[1]);
        }

    public:
        TacticalCache(TacticalCache const&) = delete;
        TacticalCache& operator= (TacticalCache const&) = delete;
        constexpr TacticalCache(){
            for(auto &primary_tier: {0, 1, 2, 3, 4, 5}){
                for(auto &secondary_tier: {0, 1, 2, 3, 4, 5}){
                    for(auto &primary_dual: {0, 1}){
                        for(auto &secondary_dual: {0, 1}){
                            auto primary = Threat(primary_tier, primary_dual);
                            auto secondary = Threat(secondary_tier, secondary_dual);
                            auto id = ThreatPairToID(primary, secondary);
                            m_data[id] = TacticSet(primary, secondary);
                        }
                    }
                }
            }
        }

        static const TacticSet& Get(const SortedThreatSet& threats){
            return m_instance.m_data[ThreatSetToID(threats)];
        }
    };
}


#endif //XO_TACTICALCACHE_H
