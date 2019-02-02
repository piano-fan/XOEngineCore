#include "squarescore.h"


namespace XO{
    void SquareScore::Calculate(const SquareInfluence &infl, Piece p){
        if(infl.GetTactics(p).GetProperty(TProperty::FORK)){
            m_fields.m_tactical = infl.GetTactics(p).Value();
            m_fields.m_primary_tier = 0;
            m_fields.m_secondary_tier = 0;
            m_fields.m_primary_score = 0;
            m_fields.m_secondary_score = 0;
            return;
        }

        const auto& threats = infl.GetThreats(p);
        //TODO: объединить 2 вызова в один, вычислять у кого инициатива
        ValueT top_tier = threats[0].GetMixedTier();
        ValueT top_score = threats[0].GetScore();

        ValueT bot_tier = threats[0].GetMixedTier();
        ValueT bot_score = 0;

        int i = 1;
        for(; i < 4; ++i){
            ValueT next_tier = threats[i].GetMixedTier();

            bot_tier = next_tier;
            bot_score = threats[i].GetScore();

            if(next_tier >= top_tier){
                top_score += threats[i].GetScore() + 32;
            }
            else{
                ++i;
                break;
            }
        }

        for(; i < 4; ++i){
            ValueT next_tier = threats[i].GetMixedTier();
            if(next_tier >= bot_tier){
                bot_score += threats[i].GetScore();
            }
        }

        m_fields.m_tactical = 0;
        m_fields.m_primary_tier = top_tier;
        m_fields.m_secondary_tier = bot_tier;
        m_fields.m_primary_score = top_score;
        m_fields.m_secondary_score = bot_score;
    }
}
