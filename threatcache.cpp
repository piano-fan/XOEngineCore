#include "threatcache.h"
#include "oldthreatcache.h"
#include <cassert>


namespace XO{
    ThreatInfoCache::ThreatInfoCache()
        :m_data(std::make_unique<std::array<Threat, TOTAL_MASK_COUNT>>())
    {}

    void ThreatInfoCache::Init(Piece p)
    {
        auto mask = PieceSet8();
        auto old_instance = std::make_unique<Old::ThreatCache_1_0>(p);

        do{
            auto single = (*old_instance)[mask].GetCRef(Old::SINGLE_THREAT);
            auto dual = (*old_instance)[mask].GetCRef(Old::DUAL_THREAT);

            ValueT singletier = static_cast<ValueT>(single.GetTier());
            ValueT dualtier = static_cast<ValueT>(dual.GetTier()) + 1;
            ValueT maxtier = std::max(singletier, dualtier);

            Ref(mask).SetTier(static_cast<ThreatTier>(maxtier));

            if(dualtier > singletier){
                Ref(mask).SetDual(1);
                Ref(mask).SetScore(dual.GetScore());
            }
            else if(dualtier == singletier){
                Ref(mask).SetDual(0);
                Ref(mask).SetScore(dual.GetScore()); //TODO: здесь ошибка - не учитываются single клетки
            }
            else{
                assert(dual.GetTier() < single.GetTier());
                Ref(mask).SetDual(0);
                Ref(mask).SetScore(static_cast<ValueT>(dual.GetTier()));  //TODO: исследовать улучшения
            }
        }while(++mask);
    }

    ThreatInfoCache ThreatInfoCache::m_instance[2];

    void ThreatInfoCache::Init(){
        m_instance[X].Init(X);
        m_instance[O].Init(O);
    }
}
