#ifndef XO_PIECESETCACHE_H
#define XO_PIECESETCACHE_H

#include "squareinfluence.h"
#include "squaretracker.h"


namespace XO{
    class FieldMetrics;
    class SquareObserver{
        const FieldMetrics& m_metrics;

        static std::vector<SquareInfluence> m_data_reset_backup;
        using SqDataStackNode = std::array<SquareInfluence, StarOffset::TOTAL_OFFSETS>;
        using SqDataStack = std::vector<SqDataStackNode>;

        std::vector<SquareInfluence> m_data;
        std::vector<Piece> m_pieces;
        std::vector<Point> m_squares;
        SquareTracker<6> m_sq_tracker;
        std::unique_ptr<SqDataStack> m_sqdata_stack;
        DValueT m_movecount;

        Piece& GetPieceRef(Point t){
            return m_pieces[t.GetID()];
        }

        SquareInfluence& GetInfluenceRef(Point t){
            return m_data[t.GetID()];
        }

        const SquareInfluence& GetInfluenceCRef(Point t) const{
            return m_data[t.GetID()];
        }

        SquareInfluence& GetInfluenceBackupRef(StarOffset offset){
            return (*m_sqdata_stack)[m_movecount][offset.ID()];
        }

        void InitTrackedSquare(const Point& t, const SquareInfluence& current){
            m_sq_tracker.Update(Move(t, ALLY), TacticSet(), current.GetTactics(ALLY));
            m_sq_tracker.Update(Move(t, ENEMY), TacticSet(), current.GetTactics(ENEMY));
        }

        void PromoteSquare(const Point& t, const SquareInfluence& before, const SquareInfluence& after){
            m_sq_tracker.Update(Move(t, ALLY), before.GetTactics(ALLY), after.GetTactics(ALLY));
            m_sq_tracker.Update(Move(t, ENEMY), before.GetTactics(ENEMY), after.GetTactics(ENEMY));
        }

        void ReleaseTrackedSquare(const Point& t, const SquareInfluence& before){
            m_sq_tracker.Update(Move(t, ALLY), before.GetTactics(ALLY), TacticSet());
            m_sq_tracker.Update(Move(t, ENEMY), before.GetTactics(ENEMY), TacticSet());
        }

    public:
        SquareObserver(const FieldMetrics& m)
                :m_sqdata_stack(std::make_unique<SqDataStack>())
                , m_metrics(m)
        {}

        const SquareInfluence& GetInfluence(Point t) const{
            return GetInfluenceCRef(t);
        }

        Piece GetPiece(Point t) const{
            return m_pieces[t.GetID()];
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
            return GetInfluenceCRef(m.GetPos()).GetThreats(m.GetTurn())[0].IsWin();
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

        const SquareTracker_Container& GetTrackedSquares(Piece p, TProperty square_property) const{
            return m_sq_tracker.Get(p, static_cast<ValueT>(square_property));
        }

        void NotifySetPiece(const Move& m);
        void NotifyRemovePiece(const Move& m);
        void NotifyReset();
        void NotifyResize(ValueT w, ValueT h);

        std::string ToString(Point t) const{
            std::string result = GetInfluenceCRef(t).ToString();
            result += "\n" + PieceName[ALLY] + ": " + m_sq_tracker.ToString(ALLY);
            result += "\n" + PieceName[ENEMY] + ": " + m_sq_tracker.ToString(ENEMY);
            return result;
        }
    };
}

#endif // XO_PIECESETCACHE_H
