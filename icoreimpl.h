#ifndef XO_ICOREPUB_H
#define XO_ICOREPUB_H

#include "squareobserver.h"
#include "icore.h"


namespace XO{
    class ICoreImpl : public ICore{
        SquareObserver m_sq_observer;
        FieldMetrics m_metrics;
    public:
        ICoreImpl();

        void Init() override;
        void NewGame() override;
        void Resize(unsigned int w, unsigned int h) override;
        void Set(unsigned int x, unsigned int y, GomocupStoneID i) override;

        BestMoveInfo MakeBestMove(GomocupStoneID i, bool want_report) override;
        bool IsOver(unsigned int x, unsigned int y, GomocupStoneID i) const override;

        std::tuple<std::string, std::string, std::string> About() const override;
        std::string GetSquareCacheRepr(unsigned int, unsigned int) const override;
    };
}

#endif // XO_ICOREPUB_H
