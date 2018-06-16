#include "engineinfo.h"
#include "icoreimpl.h"


namespace XO{
    void ICoreImpl::Init(){
        XO::Init();
        NewGame();
    }

    void ICoreImpl::NewGame(){
        m_game = std::make_unique<Game>();
    }

    void ICoreImpl::Resize(unsigned int w, unsigned int h){
        m_game = std::make_unique<Game>(w, h);
    }

    void ICoreImpl::Set(unsigned int x, unsigned int y, GomocupStoneID i){
        m_game->MakeMove(x, y);
    }

    BestMoveInfo ICoreImpl::MakeBestMove(GomocupStoneID i, bool want_report){
        FieldIterator target = m_game->DumbBestMove();
        m_game->MakeMove(target);

        BestMoveInfo result;
        result.x = target.GetX();
        result.y = target.GetY();
        result.variation_count = 0;
        result.max_depth_reached = 0;
        result.custom_info = "";

        return result;
    }

    bool ICoreImpl::IsOver(unsigned int x, unsigned int y, GomocupStoneID i) const {
        return m_game->GameOver();
    }

    std::tuple<std::string, std::string, std::string> ICoreImpl::About() const{
        return {EngineInfo::Name(), EngineInfo::Version(), EngineInfo::Author()};
    };

    std::string ICoreImpl::GetSquareCacheRepr(unsigned int x, unsigned int y) const{
        return m_game->SquareToString(x, y);
    }
    std::unique_ptr<ICore> MakeCore()
    {
        return std::unique_ptr<ICore>(new ICoreImpl);
    };
}
