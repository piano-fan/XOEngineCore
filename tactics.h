#ifndef XO_TACTICS_H
#define XO_TACTICS_H

#include "branch.h"


namespace XO{
    class Tactics : public BaseEvaluator{
    public:
        void operator()(Data& links, Piece own) const override{
            Scan4SOnly<StaticTacticsAdapter>()(links, own);
        }
    };
}

#endif //XO_TACTICS_H
