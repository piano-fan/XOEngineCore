#ifndef XO_SQUAREINFLUENCE_H
#define XO_SQUAREINFLUENCE_H

#include "types.h"
#include "pieceset.h"
#include "sortedthreatset.h"
#include "tacticset.h"


namespace XO{
    class SquareInfluence{
        std::array<PieceSet8, 4> m_psets;
        std::array<SortedThreatSet, 2> m_threats;
        std::array<TacticSet, 2> m_tactics;

        void UpdateThreats(ValueT dir);
        void UpdateTactics();

    public:
        void ClearPSets(){
            m_psets.fill(PieceSet8());
        }
        void AddBounds(StarOffset where){
            m_psets[where.Angle()].SetPiece(OUT_OF_BOUNDS, where.Offset());
        }

        void Calculate();
        void OnNeighbourChanged(Piece p, StarOffset self);

        const PieceSet8& GetPieceSet(ValueT dir) const{
            return m_psets[dir];
        }
        const SortedThreatSet& GetThreats(Piece p) const{
            return m_threats[p];
        }
        const TacticSet& GetTactics(Piece p) const{
            return m_tactics[p];
        }

        std::string ToString() const{
            return GetPieceSet(0).ToString() + "\n"
                   + GetPieceSet(1).ToString() + "\n"
                   + GetPieceSet(2).ToString() + "\n"
                   + GetPieceSet(3).ToString() + "\n"
                   + PieceName[ALLY] + ": " + GetThreats(ALLY).ToString()
                   + PieceName[ENEMY] + ": " + GetThreats(ENEMY).ToString()
                   + GetTactics(ALLY).ToString() + "\n"
                   + GetTactics(ENEMY).ToString() + "\n";
        }
    };
}


#endif //XO_SQUAREINFLUENCE_H
