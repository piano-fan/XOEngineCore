#ifndef VARIATION_TEMPLATE_H
#define VARIATION_TEMPLATE_H

#include "position_data.h"
#include "variationmanager.h"


namespace XO{
    class IVariationCalculator{
    public:
        virtual void Calculate(PositionData& target, VariationManager& mgr, Piece turn) const = 0;
        virtual ~IVariationCalculator() = default;
    };

    class Variation : public Move{
        PositionData* m_data;

    public:
        Variation(const IVariationCalculator& op, VariationManager& mgr, const Move& mv)
            :Move(mv)
        {
            auto& hook = mgr.GetPositionCacheHook(mv);
            bool calculated = true;
            if(!hook){
                hook = std::make_unique<PositionData>();
                calculated = false;
            }
            m_data = static_cast<PositionData*>(hook.get());
            if(!m_data->Valid(mgr)){
                *m_data = PositionData();
                calculated = false;
            }

            Piece turn = OppositePiece(mv.GetTurn());

            if(!calculated){
                mgr.MakeMove(mv);
                op.Calculate(GetData(), mgr, turn);
                mgr.TakeBack(mv);
            }
        }

        PositionData& GetData(){
            return *m_data;
        }

        const PositionData& GetData() const{
            return *m_data;
        }
    };
}

#endif // VARIATION_TEMPLATE_H
