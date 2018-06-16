#include "threatinfo.h"
#include "square.h"
#include <cassert>


namespace XO{
    ThreatInfo::ThreatInfo():
        m_tier(NO_THREAT), m_score(0)
    {}

    /*void ThreatInfo::Print() const{
        std::cout << "Tier:" << static_cast<int>(m_tier) << " Score:" << static_cast<int>(m_score) << " ";
    }*/

    void ThreatInfo::Set(ThreatTier tier, ValueT score){
        m_tier = tier;
        m_score = score;
    }

    void ThreatInfo::SetTier(ThreatTier tier){
        m_tier = tier;
    }

    void ThreatInfo::SetScore(ValueT score){
        m_score = score;
    }

    ThreatTier ThreatInfo::GetTier() const{
        return static_cast<ThreatTier>(m_tier);
    }

    ValueT ThreatInfo::GetScore() const{
        return m_score;
    }


    ThreatInfoSet::ThreatInfoSet(){
        m_data.fill(ThreatInfo());
    }

    ThreatInfo& ThreatInfoSet::GetRef(Piece p, ThreatStrength s){
        return m_data[(s << 1) + p];
    }

    const ThreatInfo& ThreatInfoSet::GetCRef(Piece p, ThreatStrength s) const{
        return m_data[(s << 1) + p];
    }

    /*void ThreatInfoSet::Print(Piece p, ThreatStrength s) const{
        std::cout << PieceName[p] << ": " << ThreatStrengthName[s] << ": ";
        GetCRef(p, s).Print();
    }

    void ThreatInfoSet::Print() const{
        for(Piece &p: std::array<Piece, 2>{X, O}) {
            for (ThreatStrength &s: std::array<ThreatStrength, 2>{SINGLE_THREAT, DUAL_THREAT}) {
                Print(p, s);
            }
            std::cout << std::endl;
        }
    }*/
}
