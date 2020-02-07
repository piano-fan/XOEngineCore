#ifndef XO_EVALUATIONMANAGER_H
#define XO_EVALUATIONMANAGER_H

#include "statictactics.h"
#include "tactics_deprecated.h"
#include "staticevaluation.h"
#include "report.h"
#include "variationmanager.h"
#include "variationscanner.h"


namespace XO{
    class EvaluationManager{
        SquareObserver& m_obs;

        VariationManager m_mgr;

        bool m_have_position_data;
        PositionData m_root;

    public:
        EvaluationManager(SquareObserver& obs)
            :m_obs(obs), m_mgr(obs)
        {}

        void Reset(){
            m_have_position_data = false;
            m_root = PositionData();
        }

        void ResetStats(){
            m_obs.ResetIterationCounter();
            m_mgr.ResetMaxDepthTracker();
        }

        void Calculate(Piece turn){
            const auto& obs = m_obs;
            auto& mgr = m_mgr;

            if(obs.GetMoveCount() == 0){
                m_root.SetReport(Report{
                        Report::Author::STATIC_EVALUATION
                        , Report::Type::NONE
                        , Move(obs.Metrics().Middle(), turn)
                        , 0
                });
                return;
            }

            if(Move mv; StaticTactics::SingleMove(mv, obs, turn)){
                m_root.SetReport(Report{
                        Report::Author::STATIC_TACTICS
                        , Report::Type::NONE
                        , mv
                        , 2
                });
                return;
            }

            static constexpr DepthT MAX_DEPTH_LIMIT = 18;

            mgr.Reset(turn);
            mgr.Alloc(MAX_DEPTH_LIMIT + 2);

            for(DepthT depth = 2; depth <= MAX_DEPTH_LIMIT; depth += 2){

                mgr.SetDepthLimit(depth);
                ForcedWinCalculator().Calculate(m_root, mgr, turn);

                const auto& report = m_root.GetReport();

                if(report.Final()){
                    m_have_position_data = true;
                    return;
                }
                if(!report.DepthLimit()){
                    break;
                }
            }

            if(Move result; Tactics_Deprecated()(result, obs, turn)){
                m_root.SetReport(Report{
                        Report::Author::DEPRECATED_TACTICS
                        , Report::Type::NONE
                        , result
                        , 0
                });
                return;
            }

            {
                StaticEvaluator::Report report;
                StaticEvaluator()(report, obs, turn);
                m_root.SetReport(Report{
                        Report::Author::STATIC_EVALUATION
                        , report.attacker == ALLY ? Report::Type::SUCCESS
                                                  : Report::Type::FAIL
                        , report.m
                        , 0
                });
                return;
            }
        }

        const Move& GetMove() const{
            if(m_have_position_data){
                return m_root.m_best_move;
            }
            return m_root.GetReport().move;
        }

        uint64_t GetIterationCount() const{
            return m_obs.GetIterationCount();
        }

        uint64_t GetPositionCount() const{
            return m_mgr.GetPositionCount();
        }

        uint64_t GetMaxAchievedDepth() const{
            return m_mgr.GetMaxAchievedDepth();
        }

        std::string ReportToString() const{
            if(m_have_position_data){
                return m_root.ToString();
            }
            return m_root.GetReport().ToString();
        }
    };
}

#endif //XO_EVALUATIONMANAGER_H
