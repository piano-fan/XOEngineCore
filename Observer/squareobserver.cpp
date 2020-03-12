#include "squareobserver.h"


namespace XO{
    std::vector<SquareInfluence> SquareObserver::m_data_reset_backup;


    void SquareObserver::NotifySetPiece(const Move& m){
        GetPieceRef(m.GetPos()) = m.GetTurn();
        ReleaseTrackedSquare(m.GetPos(), GetInfluence(m.GetPos()));
        m_pos_hash.SetPiece(m);
        for(auto current = StarOffset::Begin(); current.Valid(); ++current){
            Point t = Metrics().MakePoint(m.GetPos(), current);
            if(!Metrics().InBounds(t)){
                continue;
            }

            GetInfluenceBackupRef(current) = GetInfluence(t);
            GetInfluenceRef(t).OnNeighbourChanged(m.GetTurn(), current);
            if(GetPiece(t) == EMPTY){
                PromoteSquare(t, GetInfluenceBackupRef(current), GetInfluence(t));
            }
        }
        ++m_movecount;
        ++m_iteration_counter;
    }

    void SquareObserver::NotifyRemovePiece(const Move& m){
        --m_movecount;
        GetPieceRef(m.GetPos()) = EMPTY;
        InitTrackedSquare(m.GetPos(), GetInfluence(m.GetPos()));
        m_pos_hash.RemovePiece(m);
        for(auto current = StarOffset::Begin(); current.Valid(); ++current){
            Point t = Metrics().MakePoint(m.GetPos(), current);
            if(!Metrics().InBounds(t)){
                continue;
            }
            if(GetPiece(t) == EMPTY){
                PromoteSquare(t, GetInfluence(t), GetInfluenceBackupRef(current));
            }
            GetInfluenceRef(t) = GetInfluenceBackupRef(current);
        }
    }

    void SquareObserver::NotifyReset(){
        m_data = m_data_reset_backup;
        m_pos_hash.Clear();
        m_pieces.assign(m_pieces.size(), EMPTY);
        m_sq_tracker.Clear();
        m_movecount = 0;
        ResetIterationCounter();
    }

    void SquareObserver::NotifyResize(ValueT w, ValueT h){
        m_data.resize(Metrics().GetSquareCount());
        m_pos_hash.Resize(w * h);
        m_pieces.resize(Metrics().GetSquareCount(), EMPTY);
        m_sqdata_stack->resize(Metrics().GetSquareCount());
        Metrics().MakePoints(m_squares);
        m_sq_tracker.Clear();
        m_sq_tracker.Alloc(Metrics().GetSquareCount());

        m_movecount = 0;
        ResetIterationCounter();
        for (const Point &t: m_squares) {
            auto& sqdata = GetInfluenceRef(t);
            sqdata.ClearPSets();

            for(auto current = StarOffset::Begin(); current.Valid(); ++current){
                auto sq = Metrics().MakePoint(t, current);
                if(!Metrics().InBounds(sq)){
                    sqdata.AddBounds(current);
                    //TODO: достаточно отметить границу 1 раз
                    //и не заполнять все клетки за границей
                }
            }
            sqdata.Calculate();
        }

        m_data_reset_backup = m_data;
    }
}
