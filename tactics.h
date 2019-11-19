#ifndef XO_TACTICS_H
#define XO_TACTICS_H

#include "branch.h"


namespace XO{
    class Tactics : public BaseEvaluator{
    public:
        void operator()(EvaluationReport& r_result, VariationManager& mgr, Piece own) const override{
            Scan4S3D<StaticTacticsAdapter>()(r_result, mgr, own);
        }
    };
}

#endif //XO_TACTICS_H
