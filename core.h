#ifndef XO_CORE_H
#define XO_CORE_H

#include "types.h"
#include "gameobserver.h"
#include "piecesetcache.h"
#include "squarescorecache.h"
#include "positionscorecache.h"
#include "gamestate.h"
#include <tuple>


namespace XO{
    class Core  : public GameObserverSubject{
    protected:
        GameState m_state;
        PieceSetCache m_mask_cache;
        SquareScoreCache m_square_cache;
        PositionScoreCache m_position_cache;

        friend class PositionScore;
    public:
        Core();
        void Init();

        void NewGame();
        void Resize(unsigned int w, unsigned int h);
        void MakeMove(Point p);
        void TakeBack(Point p);
        bool GameIsOver() const;
        Point BestMove(Piece turn) const;

        bool IsWin() const;
        bool IsOver() const;

        std::string SquareToString(Point t) const;
    };
}


#endif //XO_CORE_H
