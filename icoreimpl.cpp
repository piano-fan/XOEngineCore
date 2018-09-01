#include "engineinfo.h"
#include "icoreimpl.h"


namespace XO{
    void ICoreImpl::Init(){
        Core::Init();
    }

    void ICoreImpl::NewGame(){
        Core::NewGame();
    }

    void ICoreImpl::Resize(unsigned int w, unsigned int h){
        if(m_state.GetMetrics().GetWidth() != w
                || m_state.GetMetrics().GetHeight() != h){
            Core::Resize(w, h);
        }
        else{
            Core::NewGame();
        }
    }

    //WARNING! 2.0 and older versions supports ordered move sequences only
    void ICoreImpl::Set(unsigned int x, unsigned int y, GomocupStoneID i){
        Core::MakeMove(m_state.GetMetrics().MakePoint(x, y));
    }

    BestMoveInfo ICoreImpl::MakeBestMove(GomocupStoneID i, bool want_report){
        auto bestmove = Core::BestMove(m_state.GetTurn());
        Core::MakeMove(bestmove);

        BestMoveInfo result;
        result.x = bestmove.GetX();
        result.y = bestmove.GetY();
        result.variation_count = 0;
        result.max_depth_reached = 0;
        result.custom_info = "";

        return result;
    }

    bool ICoreImpl::IsOver(unsigned int x, unsigned int y, GomocupStoneID i) const {
        return Core::IsOver();
    }

    std::tuple<std::string, std::string, std::string> ICoreImpl::About() const{
        return {EngineInfo::Name(), EngineInfo::Version(), EngineInfo::Author()};
    };

    std::string ICoreImpl::GetSquareCacheRepr(unsigned int x, unsigned int y) const{
        return Core::SquareToString(m_state.GetMetrics().MakePoint(x, y));
    }
    std::unique_ptr<ICore> MakeCore()
    {
        return std::unique_ptr<ICore>(new ICoreImpl);
    };
}
