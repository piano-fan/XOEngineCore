#include "squarescore.h"
#include "threatcache.h"
#include <cassert>
#include <algorithm>


namespace XO{
    void SquareScore::Calculate(Piece p, const std::array<PieceSet8, 4> &masks){
        assert(!Valid());
        m_tactical_tier = TacticalSquareTier::NONE;
        m_positional_tier = PositionalSquareTier::NONE;
        m_score = 0;

        //TODO: попробовать сортировку масок по убыванию силы, сохраняя номер направления
        //      дать интерфейс, показывающий какая маска является самой сильной

        ConsumeMask(p, masks[0]);
        ConsumeMask(p, masks[1]);
        ConsumeMask(p, masks[2]);
        ConsumeMask(p, masks[3]);

        if(m_positional_tier == PositionalSquareTier::FORCED){
            m_value = static_cast<int>(m_tactical_tier) << 16;
        }
        else{
            m_value = (static_cast<int>(m_positional_tier) << 8) + m_score;
        }

        m_valid = true;
    }

    std::string SquareScore::ToString() const {
        assert(Valid());
        return std::string("T:") + std::to_string(static_cast<int>(m_tactical_tier)) + " P:" + std::to_string(static_cast<int>(m_positional_tier))
                  + " Sc:" + std::to_string(static_cast<int>(m_score)) + " Val:" + std::to_string(Value());
    }

    void SquareScore::ConsumeMask(Piece p, PieceSet8 m) {
        const auto& nextinfo = ThreatInfoCache::Instance(p)[m];
        auto nexttier = nextinfo.GetTier();

        switch(nexttier){
            case ThreatTier::WIN:
                if(!nextinfo.GetDual()) {
                    m_tactical_tier = TacticalSquareTier::WIN;
                    m_positional_tier = PositionalSquareTier::FORCED;
                }
                else if(m_tactical_tier != TacticalSquareTier::WIN){
                    m_tactical_tier = TacticalSquareTier::UPPER_DUAL;
                    m_positional_tier = PositionalSquareTier::FORCED;
                }
                break;
            case ThreatTier::WIN_THREAT:
                switch(m_tactical_tier){
                    case TacticalSquareTier::WIN:
                        assert(false);
                        return;
                    case TacticalSquareTier::UPPER_DUAL:
                        break;
                    case TacticalSquareTier::SINGLE_N_LOWER_DUAL:
                        if(!nextinfo.GetDual()){
                            m_tactical_tier = TacticalSquareTier::UPPER_DUAL;
                            m_positional_tier = PositionalSquareTier::FORCED;
                        }
                        break;
                    case TacticalSquareTier::DOUBLE_LOWER_DUAL:
                        if(!nextinfo.GetDual()){
                            m_tactical_tier = TacticalSquareTier::SINGLE_N_LOWER_DUAL;
                            m_positional_tier = PositionalSquareTier::FORCED;
                        }
                        break;
                    case TacticalSquareTier::SINGLE:
                        if(nextinfo.GetDual()){
                            m_tactical_tier = TacticalSquareTier::SINGLE_N_LOWER_DUAL;
                            m_positional_tier = PositionalSquareTier::FORCED;
                        }
                        else{
                            m_tactical_tier = TacticalSquareTier::UPPER_DUAL;
                            m_positional_tier = PositionalSquareTier::FORCED;
                        }
                        break;
                    case TacticalSquareTier::LOWER_DUAL:
                        if(nextinfo.GetDual()){
                            m_tactical_tier = TacticalSquareTier::DOUBLE_LOWER_DUAL;
                            m_positional_tier = PositionalSquareTier::FORCED;
                        }
                        else{
                            m_tactical_tier = TacticalSquareTier::SINGLE_N_LOWER_DUAL;
                            m_positional_tier = PositionalSquareTier::FORCED;
                        }
                        break;
                    case TacticalSquareTier::NOT_TACTICAL:    //TODO: Extend & evaluate tactics (2 + 4 tactics)
                    case TacticalSquareTier::NONE:
                        if(nextinfo.GetDual()){
                            m_tactical_tier = TacticalSquareTier::LOWER_DUAL;
                        }
                        else{
                            m_tactical_tier = TacticalSquareTier::SINGLE;
                        }
                        break;
                }
                break;
            case ThreatTier::PRE_WIN_THREAT:
            case ThreatTier::HAVE_ALLY:
            case ThreatTier::CAN_WIN:
                //TODO: Extend & evaluate tactics (2 + 4 tactics)
                m_tactical_tier = std::max(TacticalSquareTier::NOT_TACTICAL, m_tactical_tier);
                break;
            case ThreatTier::NO_THREAT:
                break;
        }

        if(m_positional_tier == PositionalSquareTier::FORCED || nexttier == ThreatTier::NO_THREAT){
            return;
        }


        //TODO: обеспечить верный расчет m_score (найти ошибки - почему расчет зависит от порядка масок)

        switch(nexttier){
            case ThreatTier::WIN:
                assert(false);
                break;
            case ThreatTier::WIN_THREAT:
                switch(m_positional_tier){
                    case PositionalSquareTier::FORCED:
                        assert(false);
                        break;
                    case PositionalSquareTier::CONNECTED_N_DUAL_THREAT:
                    case PositionalSquareTier::CONNECTED_N_SINGLE_THREAT:
                    case PositionalSquareTier::DUAL_THREAT:
                    case PositionalSquareTier::SINGLE_THREAT:
                        assert(false);
                        break;
                    case PositionalSquareTier::CONNECTED:
                        if(nextinfo.GetDual()){
                            m_positional_tier = PositionalSquareTier::CONNECTED_N_DUAL_THREAT;
                            //TODO: update mixed score
                        }
                        else{
                            m_positional_tier = PositionalSquareTier::CONNECTED_N_SINGLE_THREAT;
                        }
                        break;
                    case PositionalSquareTier::CAN_DUAL:
                    case PositionalSquareTier::CAN_SINGLE:
                    case PositionalSquareTier::NONE:
                        if(nextinfo.GetDual()){
                            m_positional_tier = PositionalSquareTier::DUAL_THREAT;
                            m_score = nextinfo.GetScore();
                            //TODO: update mixed score
                        }
                        else{
                            m_positional_tier = PositionalSquareTier::SINGLE_THREAT;
                        }
                        break;
                }
                break;
            case ThreatTier::PRE_WIN_THREAT:
                switch(m_positional_tier){
                    case PositionalSquareTier::FORCED:
                        assert(false);
                        break;
                    case PositionalSquareTier::CONNECTED_N_DUAL_THREAT:
                    case PositionalSquareTier::CONNECTED_N_SINGLE_THREAT:
                        m_score += nextinfo.GetScore();   //TODO: check
                        break;
                    case PositionalSquareTier::DUAL_THREAT:
                        m_positional_tier = PositionalSquareTier::CONNECTED_N_DUAL_THREAT;
                        m_score = nextinfo.GetScore();
                        break;
                    case PositionalSquareTier::SINGLE_THREAT:
                        m_positional_tier = PositionalSquareTier::CONNECTED_N_SINGLE_THREAT;
                        m_score = nextinfo.GetScore();
                        break;
                    case PositionalSquareTier::CONNECTED:
                        m_score += nextinfo.GetScore();   //TODO: dual_connected?
                        break;
                    case PositionalSquareTier::CAN_DUAL:
                    case PositionalSquareTier::CAN_SINGLE:
                    case PositionalSquareTier::NONE:
                        m_positional_tier = PositionalSquareTier::CONNECTED;
                        m_score = nextinfo.GetScore();
                        break;
                }
                break;
            case ThreatTier::HAVE_ALLY:
                switch(m_positional_tier){
                    case PositionalSquareTier::FORCED:
                        assert(false);
                        break;
                    case PositionalSquareTier::CONNECTED_N_DUAL_THREAT:
                    case PositionalSquareTier::CONNECTED_N_SINGLE_THREAT:
                    case PositionalSquareTier::DUAL_THREAT:
                    case PositionalSquareTier::SINGLE_THREAT:
                    case PositionalSquareTier::CONNECTED:
                        break;
                    case PositionalSquareTier::CAN_DUAL:
                        m_score += nextinfo.GetScore();
                        break;
                    case PositionalSquareTier::CAN_SINGLE:
                        if(nextinfo.GetDual()){
                            m_positional_tier = PositionalSquareTier::CAN_DUAL;
                        }
                        m_score += nextinfo.GetScore();
                        break;
                    case PositionalSquareTier::NONE:
                        if(nextinfo.GetDual()){
                            m_positional_tier = PositionalSquareTier::CAN_DUAL;
                        }
                        else{
                            m_positional_tier = PositionalSquareTier::CAN_SINGLE;
                        }
                        m_score = nextinfo.GetScore();
                        break;
                }
                break;
            case ThreatTier::CAN_WIN:
                switch(m_positional_tier){
                    case PositionalSquareTier::FORCED:
                        assert(false);
                        break;
                    case PositionalSquareTier::CONNECTED_N_DUAL_THREAT:
                    case PositionalSquareTier::CONNECTED_N_SINGLE_THREAT:
                    case PositionalSquareTier::DUAL_THREAT:
                    case PositionalSquareTier::SINGLE_THREAT:
                    case PositionalSquareTier::CONNECTED:
                    case PositionalSquareTier::CAN_DUAL:
                    case PositionalSquareTier::CAN_SINGLE:
                        m_score += nextinfo.GetScore();
                        break;
                    case PositionalSquareTier::NONE:
                        m_positional_tier = PositionalSquareTier::CAN_SINGLE;
                        m_score = nextinfo.GetScore();
                        break;
                }
                break;
            case ThreatTier::NO_THREAT:
                break;
        }
    }

    bool SquareScore::Useless() const {
        assert(Valid());
        return m_positional_tier == PositionalSquareTier::NONE;
    }

    bool SquareScore::Tactical() const {
        assert(Valid());
        return m_tactical_tier != TacticalSquareTier::NONE && m_tactical_tier != TacticalSquareTier::NOT_TACTICAL;
    }

    bool SquareScore::Forced() const {
        assert(Valid());
        return m_positional_tier == PositionalSquareTier::FORCED;
    }

    bool SquareScore::Win() const {
        assert(Valid());
        return m_tactical_tier == TacticalSquareTier::WIN;
    }

    TacticalSquareTier SquareScore::GetTacticalTier() const {
        assert(Valid());
        return m_tactical_tier;
    }

    PositionalSquareTier SquareScore::GetPositionalTier() const {
        assert(Valid());
        return m_positional_tier;
    }

    //TODO: решение проблемы с 2мя угрозами вида:
    //OOO~*X~~~ + #~~X*O~OO - здесь определяются 2 угрозы в открытой троечке + угроза в звездочке
    //        и правый край троечки не распознается как блокер для всех 3х угроз
    //реализовать функцию Square::TestNullMove()
}
