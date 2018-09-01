#ifndef XO_THREATCACHE_H
#define XO_THREATCACHE_H

#include <memory>
#include <array>
#include "pieceset.h"
#include "threatinfo.h"


namespace XO{
    class ThreatInfoCache{
        std::unique_ptr<std::array<Threat, TOTAL_MASK_COUNT>> m_data;

        ThreatInfoCache();
        void Init(Piece p);
        ThreatInfoCache(ThreatInfoCache const&) = delete;
        ThreatInfoCache& operator= (ThreatInfoCache const&) = delete;

        Threat& Ref(PieceSet8 mask){
            return (*m_data)[mask.GetID()];
        }

        static ThreatInfoCache m_instance[2];
    public:
        static void Init();

        static const ThreatInfoCache& Instance(Piece p){
            return m_instance[p];
        }
        const Threat& operator[](PieceSet8 mask) const{
            return (*m_data)[mask.GetID()];
        }
    };
}


#endif //XO_THREATCACHE_H
