#include "threatinfo.h"
#include <iostream>


namespace XO{
    namespace ThreatInfoCache {
        const int TOTAL_MASK_COUNT = 65536;
        ThreatInfo ThreatScoreTab[TOTAL_MASK_COUNT][PLAYER_COUNT];

        ThreatInfo::ValueT GetScore(PieceMask8 m, Piece p, ThreatTier t, ThreatStrength s) {
            return ThreatScoreTab[m.GetID()][GetPieceID(p)].GetScore(t, s);
        }

        ThreatInfo::ValueT IsWinSlow(PieceMask8 m, Piece p){
            Piece current = OUT_OF_BOUNDS;
            int8_t count = 0;
            for(int i = 0; i < 8; i++){
                Piece next = m.GetPiece(i);
                if(next != current){
                    count = 1;
                    current = next;
                }
                else{
                    count++;
                    if(current == p && count >= 4){
                        return 1;
                    }
                }
            }
            return 0;
        }

        ThreatInfo::ValueT TestNullMoves(PieceMask8 m, Piece p, ThreatTier t, ThreatStrength s){
            auto prev = static_cast<ThreatTier>(t-1);
            if(GetScore(m, p, prev, s)){
                return 0;
            }
            int8_t count = 0;
            for(int i = 0; i < 8; i++){
                Piece next = m.GetPiece(i);
                if(next == EMPTY){
                    PieceMask8 test = m;
                    test.SetPiece(p, i);
                    if(GetScore(test, p, prev, s)){
                        count++;
                    }
                }
            }
            if((t == WIN_THREAT) && s == DUAL_THREAT){
                return count > 1;
            }
            return count;
        }

        void Calculate(Piece p){
            PieceID id = GetPieceID(p);
            PieceMask8 i = PieceMask8();
            ThreatInfo::ValueT result;
            do{
                result = IsWinSlow(i, p);
                ThreatScoreTab[i.GetID()][id] = ThreatInfo();
                ThreatScoreTab[i.GetID()][id].SetScore(WIN, SINGLE_THREAT, result);
                ThreatScoreTab[i.GetID()][id].SetScore(WIN, DUAL_THREAT, result);
            }while(++i);

            for(int j = WIN_THREAT; j < NO_THREAT; j++) {
                auto t = static_cast<ThreatTier> (j);
                do {
                    result = TestNullMoves(i, p, t, SINGLE_THREAT);
                    ThreatScoreTab[i.GetID()][id].SetScore(t, SINGLE_THREAT, result);
                    result = TestNullMoves(i, p, t, DUAL_THREAT);
                    ThreatScoreTab[i.GetID()][id].SetScore(t, DUAL_THREAT, result);
                } while (++i);
            }

            do{
                result = 1;
                for(int j = 0; j < NO_THREAT; j++){
                    if(GetScore(i, p, static_cast<ThreatTier> (j), SINGLE_THREAT)){
                        result = 0;
                        break;
                    }
                }
                ThreatScoreTab[i.GetID()][id].SetScore(NO_THREAT, SINGLE_THREAT, result);

                result = 1;
                for(int j = 0; j < NO_THREAT; j++){
                    if(GetScore(i, p, static_cast<ThreatTier> (j), DUAL_THREAT)){
                        result = 0;
                        break;
                    }
                }
                ThreatScoreTab[i.GetID()][id].SetScore(NO_THREAT, DUAL_THREAT, result);
            }while(++i);
        }
    }


    ///////////////////////////////////////////////////////////////////////////////////////////////////////


    int ThreatInfo::TStoID(ThreatTier t, ThreatStrength s){
        return (t<<1) + s;
    }

    void ThreatInfo::Init(){
        ThreatInfoCache::Calculate(X);
        ThreatInfoCache::Calculate(O);
    }

    ThreatInfo::ThreatInfo(){
        m_ThreatScore.fill(0);
    }

    ThreatInfo::ThreatInfo(PieceMask8 m, Piece p) {
        *this = ThreatInfoCache::ThreatScoreTab[m.GetID()][GetPieceID(p)];
    }

    void ThreatInfo::Print(){
        for(int s = 0; s < THREAT_STRENGTH_COUNT; s++) {
            std::cout << ThreatStrengthName[s] << ": ";
            for (int t = 0; t < THREAT_TIER_COUNT; t++){
                std::cout << ThreatTierName[t] << " "
                          << static_cast<int>(GetScore(static_cast<ThreatTier>(t), static_cast<ThreatStrength>(s))) << " ";
            }
        }
    }

    void ThreatInfo::SetScore(ThreatTier t, ThreatStrength s, ThreatInfo::ValueT score){
        m_ThreatScore[TStoID(t,s)] = score;
    }

    ThreatInfo::ValueT ThreatInfo::GetScore(ThreatTier t, ThreatStrength s){
        return m_ThreatScore[TStoID(t,s)];
    };

    ThreatTier ThreatInfo::TopTier(ThreatStrength s){
        for(int i = 0; i < THREAT_TIER_COUNT; i++){
            if(GetScore(static_cast<ThreatTier>(i), s)){
                return static_cast<ThreatTier>(i);
            }
        }
        return NO_THREAT;
    }

    void ThreatInfo::operator+=(const ThreatInfo& ti){
        for(int i = 0; i < THREAT_TOTAL_COUNT; i++) {
            m_ThreatScore[i] += ti.m_ThreatScore[i];
        }
    }

    void ThreatInfo::operator-=(const ThreatInfo& ti){
        for(int i = 0; i < THREAT_TOTAL_COUNT; i++) {
            m_ThreatScore[i] -= ti.m_ThreatScore[i];
        }
    }

    ThreatInfo ThreatInfo::Normalize(){
        for(int i = 0; i < THREAT_TOTAL_COUNT; i++) {
            m_ThreatScore[i] = static_cast<ValueT>(m_ThreatScore[i] ? 1 : 0);
        }
        return *this;
    }
}
