#ifndef XO_PIECESETCACHE_H
#define XO_PIECESETCACHE_H

#include "squareinfluence.h"
#include "squarescore.h"


namespace XO{
    struct SquareData{
        SquareInfluence influence;
        std::array<SquareScore, 2> scores;
        Piece piece;

        void CalculateScore(){
            scores[ALLY].Calculate(influence, ALLY);
            scores[ENEMY].Calculate(influence, ENEMY);
        }

        void OnNeighbourChanged(Piece p, StarOffset self){
            influence.OnNeighbourChanged(p, self);
            CalculateScore();
        }

        std::string ToString() const{
            return influence.ToString()
                   + scores[ALLY].ToString() + "\n"
                   + scores[ENEMY].ToString() + "\n";
        }
    };

    class FieldMetrics;
    class SquareObserver{
        const FieldMetrics& m_metrics;

        static std::vector<SquareData> m_data_reset_backup;
        std::vector<SquareData> m_data;
        std::vector<Point> m_squares;
        DValueT m_movecount;

        SquareData& GetSqRef(Point t){
            return m_data[t.GetID()];
        }

        const SquareData& Get(Point t) const{
            return m_data[t.GetID()];
        }
    public:
        SquareObserver(const FieldMetrics& m)
                :m_metrics(m)
        {}

        const SquareInfluence& GetInfluence(Point t) const{
            return Get(t).influence;
        }

        const SquareScore& GetScore(Point t, Piece p) const{
            return Get(t).scores[p];
        }

        Piece GetPiece(Point t) const{
            return Get(t).piece;
        }

        const SortedThreatSet& GetThreats(const Move& m) const{
            return GetInfluence(m.GetPos()).GetThreats(m.GetTurn());
        }

        const TacticSet& GetTactics(const Move& m) const{
            return GetInfluence(m.GetPos()).GetTactics(m.GetTurn());
        }

        bool ValidMove(Point t) const{
            return Metrics().InBounds(t)
                && GetPiece(t) == EMPTY;
        }

        bool ValidMove(const Move& m) const{
            return ValidMove(m.GetPos());
        }

        bool IsWin(const Move& m) const{
            return Get(m.GetPos()).influence.GetThreats(m.GetTurn())[0].IsWin();
        }

        DValueT GetMoveCount() const{
            return m_movecount;
        }

        const std::vector<Point>& GetSquares() const{
            return m_squares;
        }

        const FieldMetrics& Metrics() const{
            return m_metrics;
        }

        void NotifySetPiece(const Move& m);
        void NotifyReset();
        void NotifyResize(ValueT w, ValueT h);

        std::string ToString(Point t) const{
            return Get(t).ToString();
        }
    };
}

#endif // XO_PIECESETCACHE_H
