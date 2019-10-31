#ifndef POSITION_DATA_H
#define POSITION_DATA_H

#include "variationmanager.h"
#include "report.h"


namespace XO{
    struct PositionData{
        const PositionData* m_best_child = nullptr;
        Move m_best_move;
        DepthT m_scan_depth;
        Report m_report;

        DepthT FullDepth() const{
            return m_report.depth + m_scan_depth;
        }

        bool Valid(VariationManager& mgr) const{
            return !m_report.DepthLimit() || m_report.depth >= mgr.GetDepthLimit();
        }

        const Report& GetReport() const{
            return m_report;
        }

        void SetReport(const Report& report){
            m_report = report;
            SetStatus(report.move, nullptr);
        }

        void SetStatus(const Move& best, const PositionData* best_child){
            m_best_move = best;  //TODO: constructor
            m_best_child = best_child;
            m_scan_depth = best_child ? best_child->m_scan_depth + 1 : 0;
        }

        void ToggleFailSuccess(){
            if(m_report.type == Report::SUCCESS){
                m_report.type = Report::FAIL;
            }
            else if(m_report.type == Report::FAIL){
                m_report.type = Report::SUCCESS;
            }
        }

        std::string MovesToString() const{
            return m_best_move.ToString() + " "
                    + (m_best_child ? m_best_child->MovesToString() : "");
        }

        std::string ToString() const{
            return MovesToString()
                    + "BDepth " + std::to_string(m_scan_depth) + " "
                    + m_report.ToString() + " \n";
        }
    };
}

#endif //POSITION_DATA_H
