#ifndef XO_THREATCACHE_H
#define XO_THREATCACHE_H

#include <array>
#include "threat.h"
#include "oldthreatcache.h"

#include <fstream>

namespace XO{
    class ThreatCache{
        std::array<std::array<Threat, PieceSet8::TOTAL_COUNT>, 2> m_data;
        static ThreatCache m_instance;
        static constexpr const char* m_filename = "threat_3_0.cache";

        constexpr Threat& Ref(Piece p, PieceSet8 mask){
            return m_data[p][mask.GetID()];
        }

        static void Dump() {
            std::fstream dump(m_filename, std::ios::binary | std::ios::out);
            dump.write(reinterpret_cast<char*>(&m_instance.m_data.front()), sizeof(m_instance.m_data));
            dump.close();
        }

        static bool Read() {
            std::fstream data(m_filename, std::ios::binary | std::ios::in);
            if(!data.is_open()) return false;
            data.read(reinterpret_cast<char*>(&m_instance.m_data.front()), sizeof(m_instance.m_data));
            data.close();
            return true;
        }

    public:
        ThreatCache(ThreatCache const&) = delete;
        ThreatCache& operator= (ThreatCache const&) = delete;
        ThreatCache(){
            if(Read()) return;
            for(auto& p: Players){
                auto mask = PieceSet8();
                Old::ThreatCache_1_0 old_instance(p);

                do{
                    auto single = old_instance[mask].GetCRef(false);
                    auto dual = old_instance[mask].GetCRef(true);

                    uint8_t singletier = single.GetTier();
                    uint8_t dualtier = dual.GetTier();
                    uint8_t maxtier = std::max(singletier, dualtier);

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
            Dump();
        }

        constexpr static const Threat& Get(Piece p, PieceSet8 mask){
            return m_instance.Ref(p, mask);
        }
    };
}


#endif //XO_THREATCACHE_H
