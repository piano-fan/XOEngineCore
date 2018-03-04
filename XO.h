#ifndef XO_XO_H
#define XO_XO_H

#include <vector>
#include <utility>
#include <functional>
#include <memory>
#include <array>
#include "field.h"
#include "fielditerator.h"
#include "threatinfo.h"
#include <string>


namespace XO {
    class SquareInfo {
        PieceMask8 m_masks[4];

        ThreatInfo m_ThreatInfo[PLAYER_COUNT];

        int m_FinisherTier[2];
        int m_PieceDistance[2];
        int m_Tactical;

        int GetFinisherTierSlow(Piece p);
        int GetPieceDistanceSlowFromMask(PieceMask8 m, Piece p);
        int GetPieceDistanceSlow(Piece p);

        void Calculate(Piece owner);
    public:

        SquareInfo(Field* f, int x, int y);
        void Print();

        int Compare(SquareInfo &s, Piece own);

        ThreatInfo GetThreatInfo(Piece p);
        int GetFinisherTier(Piece p);
        int GetPieceDistance(Piece p);
        int IsTactical();
    };

    void Init();

    class Game {
    public:
        using MoveListT = std::vector<std::pair<int, int>>;
    private:
        std::shared_ptr<Field> m_field;
        MoveListT m_movelist;
        int m_movecount = 0;
        Piece m_move;
        Piece m_winner;

    public:
        Game(unsigned int w = 16, unsigned int h = 16);
        void New(unsigned int w, unsigned int h);

        bool GameOver();
        bool XMove();
        Piece Move();
        void SwapMove();
        Piece Winner();
        int GetMoveCount();
        bool CanMove();
        const MoveListT& GetMoveList() const;

        void MakeMove(int x, int y);
        void MakeMove(const FieldIterator& pos);
        void TakeBack();

        void Print();
        void PrintSquare(int x, int y);

        SquareInfo DumbBestMove(int &r_x, int &r_y);
        SquareInfo DeepBestMove(int depth, int &r_x, int &r_y);
    };
}


#endif
