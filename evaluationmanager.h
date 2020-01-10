#ifndef XO_EVALUATIONMANAGER_H
#define XO_EVALUATIONMANAGER_H

#include "statictactics.h"
#include "tactics_deprecated.h"
#include "staticevaluation.h"
#include "report.h"
#include "variationmanager.h"


namespace XO{
    class EvaluationManager{
        SquareObserver& m_obs;

        VariationManager m_mgr;

        Report m_current_report;

    public:
        EvaluationManager(SquareObserver& obs)
            :m_obs(obs), m_mgr(obs)
        {}

        void Reset(){
            m_current_report = Report();
        }

        void ResetStats(){
            m_obs.ResetIterationCounter();
            m_mgr.ResetMaxDepthTracker();
        }

        void Calculate(Piece turn){
            const auto& obs = m_obs;

            if(obs.GetMoveCount() == 0){
                m_current_report = Report{
                        Report::Author::STATIC_EVALUATION
                        , Report::Type::NONE
                        , Move(obs.Metrics().Middle(), turn)
                        , 0
                };
                return;
            }

            if(Move mv; StaticTactics::SingleMove(mv, obs, turn)){
                m_current_report = Report{
                        Report::Author::STATIC_TACTICS
                        , Report::Type::NONE
                        , mv
                        , 2
                };
                return;
            }

            {
                StaticTactics()(m_current_report, obs, turn);
                if(m_current_report.Final()){
                    return;
                }
            }

            if(Move result; Tactics_Deprecated()(result, obs, turn)){
                m_current_report = Report{
                        Report::Author::DEPRECATED_TACTICS
                        , Report::Type::NONE
                        , result
                        , 0
                };
                return;
            }

            {
                StaticEvaluator::Report report;
                StaticEvaluator()(report, obs, turn);
                m_current_report = Report{
                        Report::Author::STATIC_EVALUATION
                        , report.attacker == ALLY ? Report::Type::SUCCESS
                                                  : Report::Type::FAIL
                        , report.m
                        , 0
                };
                return;
            }
        }

        const Move& GetMove() const{
            return m_current_report.move;
        }

        uint64_t GetIterationCount() const{
            return m_obs.GetIterationCount();
        }

        uint64_t GetPositionCount() const{
            return 0;
        }

        uint64_t GetMaxAchievedDepth() const{
            return m_mgr.GetMaxAchievedDepth();
        }

        std::string ReportToString() const{
            return m_current_report.ToString();
        }
    };
}

#endif //XO_EVALUATIONMANAGER_H
