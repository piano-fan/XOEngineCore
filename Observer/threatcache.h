#ifndef XO_THREATCACHE_H
#define XO_THREATCACHE_H

#include <memory>
#include <array>
#include "pieceset.h"
#include "threat.h"
#include "oldthreatcache.h"


namespace XO{
    class ThreatCache{
        std::array<std::array<Threat, PieceSet8::TOTAL_COUNT>, 2> m_data;
        static ThreatCache m_instance;

        constexpr Threat& Ref(Piece p, PieceSet8 mask){
            return m_data[p][mask.GetID()];
        }

    public:
        ThreatCache(ThreatCache const&) = delete;
        ThreatCache& operator= (ThreatCache const&) = delete;
        constexpr ThreatCache(){
            for(auto& p: Players){
                auto mask = PieceSet8();
                Old::ThreatCache_1_0 old_instance(p);

                do{
                    auto single = old_instance[mask].GetCRef(false);
                    auto dual = old_instance[mask].GetCRef(true);

                    ValueT singletier = single.GetTier();
                    ValueT dualtier = dual.GetTier();
                    ValueT maxtier = std::max(singletier, dualtier);

                    m_instance.Ref(p, mask).SetTier(maxtier);

                    if(dualtier == singletier){
                        if(single.GetTier() == 3){
                            m_instance.Ref(p, mask).SetScore((dual.GetScore() > 1) ? 4 : 0);
                        }
                        else{
                            m_instance.Ref(p, mask).SetScore(dual.GetScore());
                        }
                        m_instance.Ref(p, mask).SetDual(1);
                    }
                    else{
                        m_instance.Ref(p, mask).SetDual(0);
                        if(dualtier+1 == singletier){
                            m_instance.Ref(p, mask).SetSDCombo(1);
                        }
                        else{
                            m_instance.Ref(p, mask).SetSDCombo(0);
                        }
                        if(single.GetTier() == 4){
                            m_instance.Ref(p, mask).SetScore(single.GetScore());
                        }
                        else{
                            //deprecated
                            m_instance.Ref(p, mask).SetScore(dual.GetTier()); //TODO: improve
                        }
                    }
                }while(++mask);
            }
        }

        constexpr static const Threat& Get(Piece p, PieceSet8 mask){
            return m_instance.Ref(p, mask);
        }
    };
}


#endif //XO_THREATCACHE_H
