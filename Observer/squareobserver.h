#ifndef XO_PIECESETCACHE_H
#define XO_PIECESETCACHE_H

#include "squareinfluence.h"
#include "squaretracker.h"


namespace XO{
    class FieldMetrics;
    class SquareObserver{
        const FieldMetrics& m_metrics;
        std::vector<Point> m_squares;

        static std::vector<SquareInfluence> m_data_reset_backup;
        std::vector<SquareInfluence> m_data;
        std::vector<Piece> m_pieces;

        SquareTracker<6> m_sq_tracker;

        DValueT m_movecount;

        Piece& GetPieceRef(Point t){
            return m_pieces[t.GetID()];
        }

        SquareInfluence& GetInfluenceRef(Point t){
            return m_data[t.GetID()];
        }

        void PromoteSquare(const Point& t, const SquareInfluence& before, const SquareInfluence& after){
            UpdateTracker(Move(t, ALLY), before.GetTactics(ALLY), after.GetTactics(ALLY));
            UpdateTracker(Move(t, ENEMY), before.GetTactics(ENEMY), after.GetTactics(ENEMY));
        }

        void ReleaseTrackedSquare(const Point& t, const SquareInfluence& before){
            UpdateTracker(Move(t, ALLY), before.GetTactics(ALLY), TacticSet());
            UpdateTracker(Move(t, ENEMY), before.GetTactics(ENEMY), TacticSet());
        }

        void UpdateTracker(const Move& m, TacticSet before, TacticSet after){
            if(before == after){
                return;
            }
            auto promotions = after.Promotions(before);
            auto demotions = before.Promotions(after);
            for(ValueT tier = 0; tier < m_sq_tracker.TierCount(); ++tier){
                auto square_property = TProperty(tier);
                if(promotions.GetProperty(square_property)){
                    m_sq_tracker.Init(m, tier);
                }
                if(demotions.GetProperty(square_property)){
                    m_sq_tracker.Delete(m, tier);
                }
            }
        }

    public:
        SquareObserver(const FieldMetrics& m)
                :m_metrics(m)
        {}

        const SquareInfluence& GetInfluence(Point t) const{
            return m_data[t.GetID()];
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
            return GetInfluence(m.GetPos()).GetThreats(m.GetTurn())[0].IsWin();
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

        bool HaveSquares(Piece p, TProperty square_property) const{
            return !GetTrackedSquares(p, square_property).empty();
        }

        DValueT SquareCount(Piece p, TProperty square_property) const{
            return GetTrackedSquares(p, square_property).size();
        }

        Point AnySquare(Piece p, TProperty square_property) const{
            return GetTrackedSquares(p, square_property).front();
        }

        const SquareTracker_Container& GetTrackedSquares(Piece p, TProperty square_property) const{
            return m_sq_tracker.Get(p, static_cast<ValueT>(square_property));
        }

        bool Tracked(TProperty square_class) const{
            return m_sq_tracker.Tracked(static_cast<ValueT>(square_class));
        }

        void NotifySetPiece(const Move& m);
        void NotifyReset();
        void NotifyResize(ValueT w, ValueT h);

        std::string ToString(Point t) const{
            std::string result = GetInfluence(t).ToString();
            result += "\n" + PieceName[ALLY] + ": " + m_sq_tracker.ToString(ALLY);
            result += "\n" + PieceName[ENEMY] + ": " + m_sq_tracker.ToString(ENEMY);
            return result;
        }
    };
}

#endif // XO_PIECESETCACHE_H
