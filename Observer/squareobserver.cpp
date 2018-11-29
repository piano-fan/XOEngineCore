#include "squareobserver.h"


namespace XO{
    std::vector<SquareData> SquareObserver::m_data_reset_backup;


    void SquareObserver::NotifySetPiece(const Move& m){
        GetSqRef(m.GetPos()).piece = m.GetTurn();
        for(auto current = StarOffset::Begin(); current.Valid(); ++current){
            Point t = Metrics().MakePoint(m.GetPos(), current);
            if(!Metrics().InBounds(t)){
                continue;
            }
            GetSqRef(t).OnNeighbourChanged(m.GetTurn(), current);
        }
        ++m_movecount;
    }

    void SquareObserver::NotifyReset(){
        m_data = m_data_reset_backup;
        m_movecount = 0;
    }

    void SquareObserver::NotifyResize(ValueT w, ValueT h){
        m_data.resize(Metrics().GetSquareCount());
        Metrics().MakePoints(m_squares);

        m_movecount = 0;
        for (const Point &t: m_squares) {
            auto& sqdata = GetSqRef(t);
            sqdata.piece = EMPTY;
            sqdata.influence.ClearPSets();

            for(auto current = StarOffset::Begin(); current.Valid(); ++current){
                auto sq = Metrics().MakePoint(t, current);
                if(!Metrics().InBounds(sq)){
                    sqdata.influence.AddBounds(current);
                    //TODO: достаточно отметить границу 1 раз
                    //и не заполнять все клетки за границей
                }
            }
            sqdata.influence.Calculate();
            sqdata.CalculateScore();
        }

        m_data_reset_backup = m_data;
    }
}
