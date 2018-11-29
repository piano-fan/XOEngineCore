#include "engineinfo.h"
#include "icoreimpl.h"


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
        :m_sq_observer(m_metrics), m_ev_mgr(m_sq_observer)
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

    void ICoreImpl::BeginSession(){
        m_ev_mgr.ResetStats();
    }

    std::pair<unsigned int, unsigned int> ICoreImpl::GetBestMove(GomocupStoneID i){
        Piece p = GomokuStoneIDtoPiece(i);

        m_ev_mgr.Reset();
        m_ev_mgr.Calculate(p);

        auto bestmove = m_ev_mgr.GetMove().GetPos();

        return {bestmove.GetX(), bestmove.GetY()};
    }

    EngineStatus ICoreImpl::GetStatus(GomocupStoneID i){
        EngineStatus result;

        result.variation_count = m_ev_mgr.GetIterationCount();
        result.position_count = m_ev_mgr.GetPositionCount();
        result.max_depth_reached = m_ev_mgr.GetMaxAchievedDepth();
        result.custom_info = m_ev_mgr.ReportToString();

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
