#ifndef XO_XO_H
#define XO_XO_H

#include <vector>
#include <utility>
#include <memory>
#include <array>
#include "field.h"
#include <string>


namespace XO {
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

        std::string SquareToString(int x, int y);
        int BestThreat(FieldIterator &r_best, bool own = true);

        FieldIterator DumbBestMove();
    };
}


#endif
