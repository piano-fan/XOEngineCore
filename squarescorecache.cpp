#include "squarescorecache.h"
#include "directioniterator.h"


namespace XO{
    std::vector<std::array<SquareScore, 2>> SquareScoreCache::m_backup;

    SquareScoreCache::SquareScoreCache(){}

    SquareScoreCache::SquareScoreCache(const PieceSetCache& masks)
        :m_masks(&masks)
    {}

    const SquareScore& SquareScoreCache::Get(Point t, Piece p) const{
        auto &dataref = m_data[t.GetID()][p];
        if(!dataref.Valid()){
            dataref.Calculate(p, m_masks->Get(t));
        }
        return dataref;
    }

    void SquareScoreCache::Invalidate(Point t){
        for(int dir = 0; dir < 4; ++dir){
            for(auto iter = DirectionIterator(t, dir, m_metrics); iter.Valid(); ++iter){
                if(!iter.InBounds()){
                    continue;
                }
                auto &ref = m_data[iter.Current().GetID()];
                ref[X].Invalidate();
                ref[O].Invalidate();
            }
        }
    }

    void SquareScoreCache::OnMove(Point t, Piece p){
        Invalidate(t);
    }

    void SquareScoreCache::OnTakeBack(Point t){
        Invalidate(t);
    }

    void SquareScoreCache::OnNewGame(const FieldMetrics* metrics){
        if(metrics){
            auto count = metrics->GetSquareCount();
            m_data.clear();
            m_data.resize(count);
            m_metrics = metrics;

            for(const Point &t: *m_metrics){
                Get(t, X);
                Get(t, O);
            }
            m_backup = m_data;
        }
        else{
            m_data = m_backup;
        }
    }
}
