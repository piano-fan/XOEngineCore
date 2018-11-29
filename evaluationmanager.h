#ifndef XO_EVALUATIONMANAGER_H
#define XO_EVALUATIONMANAGER_H

#include "staticevaluation.h"


namespace XO{
    class EvaluationManager{
    public:
        struct Report{
            Move m;
            Piece attacker;

            std::string ToString() const{
                return std::string() + PieceName[attacker] + " initiative " + m.ToString();
            }
        };

        void operator()(Report& r_result, const SquareObserver& obs, Piece own) const{
            if(obs.GetMoveCount() == 0){
                r_result = Report{
                        Move(obs.Metrics().Middle(), own)
                        , Piece::ALLY
                };
                return;
            }
            
            {
                StaticEvaluator::Report report;
                StaticEvaluator()(report, obs, own);
                r_result = Report{
                        report.m
                        , report.attacker
                };
                return;
            }
        }
    };
}

#endif //XO_EVALUATIONMANAGER_H
