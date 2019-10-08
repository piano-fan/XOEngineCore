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
        :m_sq_observer(m_metrics), m_var_manager(m_sq_observer, m_position_hash_key)
    {}

    void ICoreImpl::Init(){}

    void ICoreImpl::NewGame(){
        m_sq_observer.NotifyReset();
        m_position_hash_key.Clear();
    }

    void ICoreImpl::Resize(unsigned int w, unsigned int h){
        if(m_metrics.GetWidth() != w
                || m_metrics.GetHeight() != h){
            m_metrics.Init(w, h);
            m_sq_observer.NotifyResize(w, h);
            m_position_hash_key.Resize(w * h);
            m_var_manager.Alloc(w * h);
        }
        else{
            m_sq_observer.NotifyReset();
            m_position_hash_key.Clear();
        }
    }

    void ICoreImpl::Set(unsigned int x, unsigned int y, GomocupStoneID i) {
        //TODO: Renju forbidden moves
        Piece p = GomokuStoneIDtoPiece(i);
        m_sq_observer.NotifySetPiece(Move(m_metrics.MakePoint(x, y), p));
        m_position_hash_key.SetPiece(Move(m_metrics.MakePoint(x, y), p));
    }

    BestMoveInfo ICoreImpl::MakeBestMove(GomocupStoneID i, bool want_report){
        Piece p = GomokuStoneIDtoPiece(i);
        BaseEvaluator::Data links(m_sq_observer, m_var_manager);
        EvaluationManager()(links, p);
        auto bestmove = links.result.moves.front();
        m_sq_observer.NotifySetPiece(bestmove);
        m_position_hash_key.SetPiece(bestmove);
        
        BestMoveInfo result;
        result.x = bestmove.GetPos().GetX();
        result.y = bestmove.GetPos().GetY();
        result.variation_count = m_var_manager.GetDepthController().ChildCount();
        result.max_depth_reached = m_var_manager.GetDepthController().HighestDepth();
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
