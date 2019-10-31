#include "engineinfo.h"
#include "icoreimpl.h"
#include "evaluationmanager.h"


namespace XO{
    Piece GomokuStoneIDtoPiece(GomocupStoneID i){
        Piece p;
        switch(i){
            case GomocupStoneID::OWN:
                p = ALLY;
                break;
            case GomocupStoneID::OPPONENT:
                p = ENEMY;
                break;
            default:
                assert(false);
                return OUT_OF_BOUNDS;
        }
        return p;
    }

    ICoreImpl::ICoreImpl()
        :m_sq_observer(m_metrics)
    {}

    void ICoreImpl::Init(){}

    void ICoreImpl::NewGame(){
        m_sq_observer.NotifyReset();
    }

    void ICoreImpl::Resize(unsigned int w, unsigned int h){
        if(m_metrics.GetWidth() != w
                || m_metrics.GetHeight() != h){
            m_metrics.Init(w, h);
            m_sq_observer.NotifyResize(w, h);
        }
        else{
            m_sq_observer.NotifyReset();
        }
    }

    void ICoreImpl::Set(unsigned int x, unsigned int y, GomocupStoneID i) {
        //TODO: Renju forbidden moves
        Piece p = GomokuStoneIDtoPiece(i);
        m_sq_observer.NotifySetPiece(Move(m_metrics.MakePoint(x, y), p));
    }

    BestMoveInfo ICoreImpl::MakeBestMove(GomocupStoneID i, bool want_report){
        Piece p = GomokuStoneIDtoPiece(i);

        BaseEvaluator::Data links(m_sq_observer);
        EvaluationManager()(links, p);

        auto bestmove = Move(links.result.point, p);
        m_sq_observer.NotifySetPiece(bestmove);
        
        BestMoveInfo result;
        result.x = bestmove.GetPos().GetX();
        result.y = bestmove.GetPos().GetY();
        result.variation_count = 0;
        result.max_depth_reached = 0;
        result.custom_info = "";
        if(want_report){
            result.custom_info = links.result.ToString();
        }

        return result;
    }

    bool ICoreImpl::IsOver(unsigned int x, unsigned int y, GomocupStoneID i) const{
        Piece p = GomokuStoneIDtoPiece(i);
        auto lastmove = Move(m_metrics.MakePoint(x, y), p);
        return (m_sq_observer.GetMoveCount() >= m_metrics.GetSquareCount())
            || m_sq_observer.IsWin(lastmove);
    }

    std::tuple<std::string, std::string, std::string> ICoreImpl::About() const{
        return {EngineInfo::Name(), EngineInfo::Version(), EngineInfo::Author()};
    };

    std::string ICoreImpl::GetSquareCacheRepr(unsigned int x, unsigned int y) const{
        return m_sq_observer.ToString(m_metrics.MakePoint(x, y));
    }
    std::unique_ptr<ICore> MakeCore()
    {
        return std::unique_ptr<ICore>(new ICoreImpl);
    };
}
