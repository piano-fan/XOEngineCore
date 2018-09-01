#include "positionscorecache.h"


namespace XO{
    PositionScoreCache::PositionScoreCache(){}

    PositionScoreCache::PositionScoreCache(Core& core):
            m_core(&core)
    {}

    const PositionScore& PositionScoreCache::Get() const{
        if(!m_pscore.Valid()){
            m_pscore.Calculate(*m_core);
        }
        return m_pscore;
    }

    void PositionScoreCache::OnMove(Point t, Piece p){
        m_pscore.Invalidate();
    }

    void PositionScoreCache::OnTakeBack(Point t){
        m_pscore.Invalidate();
    }

    void PositionScoreCache::OnNewGame(const FieldMetrics* metrics){
        m_pscore.Invalidate();   //TODO: reload
    }
}