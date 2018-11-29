#include "squarescore.h"


namespace XO{
    void SquareScore::Calculate(const SquareInfluence &infl, Piece p){
        const auto& threats = infl.GetThreats(p);
        m_positional_tier = PositionalSquareTier::NONE;
        m_score = 0;

        if(threats[0] > Threat(4, 1)   //TODO: тут на самом деле >= но score = 0 потому все работает
           || threats[1] > Threat(3, 1)) {
            m_value = (threats[0].GetValue() << 23) + (threats[1].GetValue() << 15);
            m_positional_tier = PositionalSquareTier::FORCED;
            m_score = 0;
            //TODO: эта формула выбирает клетку которая блокирует меньше возможностей врага (исправить)
            return;
        }

        ValueT top_tier = threats[0].GetMixedTier();
        ValueT bot_tier = 0;

        ValueT top_score = threats[0].GetScore();
        ValueT bot_score = 0;
        for(int i = 1; i < 4; ++i){
            ValueT next_tier = threats[i].GetMixedTier();
            if(next_tier == top_tier){
                top_score += threats[i].GetScore();
            }
            else if(next_tier == bot_tier){
                bot_score += threats[i].GetScore();
            }
            else if(bot_tier < next_tier){
                bot_tier = next_tier;
                bot_score = threats[i].GetScore();
            }
        }

        if(top_tier == 4) {
            switch (threats[1].GetMixedTier()) {
                case 3:
                    if (threats[0].GetDual()) {
                        m_positional_tier = PositionalSquareTier::CONNECTED_N_DUAL_THREAT;
                        m_score = bot_score;
                    } else {
                        m_positional_tier = PositionalSquareTier::CONNECTED_N_SINGLE_THREAT;
                        m_score = bot_score;
                    }
                    break;
                case 2:
                case 1:
                case 0:
                    if (threats[0].GetDual()) {
                        m_positional_tier = PositionalSquareTier::DUAL_THREAT;
                        m_score = top_score;
                    } else {
                        m_positional_tier = PositionalSquareTier::SINGLE_THREAT;
                        m_score = threats[1].GetTier() + bot_score;
                    }
                    break;
            }
        }
        else {
            m_positional_tier = static_cast<PositionalSquareTier>(top_tier);
            m_score = (top_score << 8) + bot_score;
        }

        m_value = (static_cast<int>(m_positional_tier) << 16) + m_score;    //TODO: unsigned int
    }
}
