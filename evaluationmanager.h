#ifndef XO_EVALUATIONMANAGER_H
#define XO_EVALUATIONMANAGER_H

#include "staticevaluation.h"


namespace XO{
    class EvaluationManager{
        SquareObserver& m_obs;

    public:
        struct Report{
            Move m;
            Piece attacker;

            std::string ToString() const{
                return std::string() + PieceName[attacker] + " initiative " + m.ToString();
            }
        };

    private:
        Report m_current_report;

    public:
        EvaluationManager(SquareObserver& obs)
            :m_obs(obs)
        {}

        void Reset(){
            m_current_report = Report();
        }

        void ResetStats(){

        }

        void Calculate(Piece turn){
            const auto& obs = m_obs;

            if(obs.GetMoveCount() == 0){
                m_current_report = Report{
                        Move(obs.Metrics().Middle(), turn)
                        , Piece::ALLY
                };
                return;
            }
            
            {
                StaticEvaluator::Report report;
                StaticEvaluator()(report, obs, turn);
                m_current_report = Report{
                        report.m
                        , report.attacker
                };
                return;
            }
        }

        const Move& GetMove() const{
            return m_current_report.m;
        }

        uint64_t GetIterationCount() const{
            return 0;
        }

        uint64_t GetPositionCount() const{
            return 0;
        }

        uint64_t GetMaxAchievedDepth() const{
            return 0;
        }

        std::string ReportToString() const{
            return m_current_report.ToString();
        }
    };
}

#endif //XO_EVALUATIONMANAGER_H
