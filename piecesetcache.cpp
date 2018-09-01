#include "piecesetcache.h"
#include "directioniterator.h"
#include <cassert>


namespace XO{
    std::vector<std::array<PieceSet8, 4>> PieceSetCache::m_backup;

    PieceSetCache::PieceSetCache(){}

    PieceSet8 PieceSetCache::Get(Point t, ValueT dir) const{
        return m_data[t.GetID()][dir];
    }

    const std::array<PieceSet8, 4>& PieceSetCache::Get(Point t) const{
        return m_data[t.GetID()];
    }

    std::string PieceSetCache::ToString(Point t) const{
        return Get(t, 0).ToString() + "\n"
               + Get(t, 1).ToString() + "\n"
               + Get(t, 2).ToString() + "\n"
               + Get(t, 3).ToString() + "\n";
    }

    void PieceSetCache::OnMove(Point t, Piece p){
        for(ValueT dir = 0; dir < 4; dir++){
            auto iter = DirectionIterator(t, dir, m_metrics);

            for( ; iter.Valid(); ++iter){
                if(!iter.InBounds()){
                    continue;
                }
                m_data[iter.Current().GetID()][dir].SetPiece(p, 7 - iter.CurrentID());
            }
        }
    }

    void PieceSetCache::OnTakeBack(Point t){
        for(ValueT dir = 0; dir < 4; dir++){
            auto iter = DirectionIterator(t, dir, m_metrics);

            for( ; iter.Valid(); ++iter){
                if(!iter.InBounds()){
                    continue;
                }
                m_data[iter.Current().GetID()][dir].ClearPiece(7 - iter.CurrentID());
            }
        }
    }

    void PieceSetCache::OnNewGame(const FieldMetrics* metrics){
        if(metrics){
            m_metrics = metrics;
            m_data.resize(m_metrics->GetSquareCount());

            auto sq_count = m_metrics->GetSquareCount();

            for(const Point& t: *m_metrics){
                for(ValueT dir = 0; dir < 4; ++dir){
                    m_data[t.GetID()][dir] = PieceSet8();
                    auto iter = DirectionIterator(t, dir, m_metrics);
                    if(iter.InBounds()){
                        auto pt_end = t;
                        pt_end += m_metrics->GetX4Offset(dir);
                        if(m_metrics->InBounds(pt_end)){
                            continue;
                        }
                    }
                    for( ; iter.Valid(); ++iter){
                        if(!iter.InBounds()){
                            m_data[t.GetID()][dir].SetPiece(OUT_OF_BOUNDS, iter.CurrentID());
                        }
                    }
                }
            }
            m_backup = m_data;
        }
        else{
            assert(m_metrics);
            m_data = m_backup;
        }
    }
}
