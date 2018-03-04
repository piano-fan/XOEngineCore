#ifndef XO_ICOREPUB_H
#define XO_ICOREPUB_H

#include "core.h"
#include "icore.h"


namespace XO{
    class ICoreImpl : public ICore, public Core{
    public:
        void Init() override;
        void NewGame() override;
        void Resize(unsigned int w, unsigned int h) override;
        void Set(unsigned int x, unsigned int y, GomocupStoneID i) override;

        void BeginSession() override;
        std::pair<unsigned int, unsigned int> GetBestMove(GomocupStoneID i) override;
        EngineStatus GetStatus(GomocupStoneID i) override;
        bool IsOver(unsigned int x, unsigned int y, GomocupStoneID i) const override;

        std::tuple<std::string, std::string, std::string> About() const override;
        std::string GetSquareCacheRepr(unsigned int, unsigned int) const override;
    };
}

#endif // XO_ICOREPUB_H
