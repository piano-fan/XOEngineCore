#ifndef XO_EVALUATIONMANAGER_H
#define XO_EVALUATIONMANAGER_H

#include "statictactics.h"
#include "tactics_deprecated.h"
#include "staticevaluation.h"


namespace XO{
    class EvaluationManager{
    public:
        struct Report{
            enum Author{
                STATIC_EVALUATION = 0, STATIC_TACTICS = 1, DEPRECATED_TACTICS = 2
            };

            Author author;
            Move m;
            Piece attacker;
            DepthT depth;

            std::string ToString() const{
                constexpr char* AuthorName[] = {
                        "Static evaluation", "Static tactics", "Deprecated tactics"
                };
                return std::string() + AuthorName[author] + ": "
                        + PieceName[attacker] + " initiative " + m.ToString()
                        + " Depth: " + std::to_string((int)depth);
            }
        };

        void operator()(Report& r_result, const SquareObserver& obs, Piece own) const{
            if(obs.GetMoveCount() == 0){
                r_result = Report{
                        Report::Author::STATIC_EVALUATION
                        , Move(obs.Metrics().Middle(), own)
                        , Piece::ALLY
                        , 1
                };
                return;
            }

            if(Move mv; StaticTactics::SingleMove(mv, obs, own)){
                r_result = Report{
                        Report::Author::STATIC_TACTICS
                        , mv
                        , Piece::ENEMY
                        , 1
                };
                return;
            }

            {
                StaticTactics::Report report;
                StaticTactics()(report, obs, own);
                if(report.winner != EMPTY){
                    assert(report.winner == ALLY || report.winner == ENEMY);
                    r_result = Report{
                            Report::Author::STATIC_TACTICS
                            , report.m
                            , report.winner
                            , report.depth
                    };
                    return;
                }
            }

            if(Move result; Tactics_Deprecated()(result, obs, own)){
                r_result = Report{
                        Report::Author::DEPRECATED_TACTICS
                        , result
                        , OUT_OF_BOUNDS
                        , 1
                };
                return;
            }

            {
                StaticEvaluator::Report report;
                StaticEvaluator()(report, obs, own);
                r_result = Report{
                        Report::Author::STATIC_EVALUATION
                        , report.m
                        , report.attacker
                        , 1
                };
                return;
            }
        }
    };
}

#endif //XO_EVALUATIONMANAGER_H
