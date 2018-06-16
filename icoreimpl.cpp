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

    void ICoreImpl::BeginSession(){

    }

    std::pair<unsigned int, unsigned int> ICoreImpl::GetBestMove(GomocupStoneID i){
        FieldIterator target = m_game->DumbBestMove();
        return {target.GetX(), target.GetY()};
    }

    EngineStatus ICoreImpl::GetStatus(GomocupStoneID i){
        EngineStatus result;

        result.variation_count = 0;
        result.position_count = 0;
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
