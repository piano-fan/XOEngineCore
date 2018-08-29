#ifndef XO_ICORE_H
#define XO_ICORE_H

#include <tuple>
#include <memory>


namespace XO{
    enum class GomocupStoneID : unsigned int{
        OWN = 1, OPPONENT = 2, FORBIDDEN = 3
    };

    struct BestMoveInfo{
        unsigned int x;
        unsigned int y;
        long variation_count;
        long max_depth_reached;
        std::string custom_info;
    };

    class ICore{
    public:
        virtual void Init() = 0;
        virtual void NewGame() = 0;
        virtual void Resize(unsigned int w, unsigned int h) = 0;
        virtual void Set(unsigned int x, unsigned int y, GomocupStoneID i) = 0;

        virtual BestMoveInfo MakeBestMove(GomocupStoneID i, bool want_report) = 0;
        virtual bool IsOver(unsigned int x, unsigned int y, GomocupStoneID i) const = 0;

        virtual std::tuple<std::string, std::string, std::string> About() const = 0;
        virtual std::string GetSquareCacheRepr(unsigned int, unsigned int) const = 0;
    };

    std::unique_ptr<ICore> MakeCore();
}

#endif //XO_ICORE_H
