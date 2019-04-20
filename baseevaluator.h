#ifndef XO_BASEEVALUATOR_H
#define XO_BASEEVALUATOR_H

#include "types.h"


namespace XO{
    struct EvaluationReport{
        enum class Mode{
            UNDEFINED = 0, STATIC_TACTICS = 1, DEEP_TACTICS = 2
            , STATIC_EVALUATION = 3, DEPRECATED = 4
        };
        enum class Type{
            NONE = 0, SUCCESS = 1, FAIL = 2, DEPTH_LIMIT = 3
        };
        static constexpr const char* evaluation_name[] = {
                "Undefined", "Static tactics", "Deep tactics", "Static evaluation", "Deprecated"
        };
        static constexpr const char* type_name[] = {
                "None", "Success", "Fail", "Depth limit"
        };
        std::list<Move> moves;
        DValueT local_depth;
        Mode mode;
        Type type;

        EvaluationReport()
                :moves(), local_depth(0), mode(Mode::UNDEFINED), type(Type::NONE)
        {}

        EvaluationReport(DValueT local_depth, Mode mode, Type type)
                :moves(), local_depth(local_depth), mode(mode), type(type)
        {}

        EvaluationReport(const Move& mv, DValueT local_depth, Mode mode, Type type)
                :moves(), local_depth(local_depth), mode(mode), type(type)
        {
            moves.push_back(mv);
        }

        void Clear(){
            new (this) EvaluationReport();
        }

        EvaluationReport& operator=(const EvaluationReport& src) = default;

        EvaluationReport& operator=(EvaluationReport&& src){
            moves = std::move(src.moves);
            local_depth = src.local_depth;
            mode = src.mode;
            type = src.type;
            return *this;
        }

        DValueT BranchDepth() const{
            return moves.size();
        }

        DValueT FullDepth() const{
            return BranchDepth() + local_depth;
        }

        bool Success() const{
            return type == Type::SUCCESS;
        }

        bool Fail() const{
            return type == Type::FAIL;
        }

        bool DepthLimit() const{
            return type == Type::DEPTH_LIMIT;
        }

        bool Final() const{
            return type != Type::NONE
                && type != Type::DEPTH_LIMIT;
        }

        bool None() const{
            return type == Type::NONE;
        }

        std::string ToString() const{
            std::string result = evaluation_name[(int)mode];
            result += ": ";
            result += type_name[(int)type];
            for(auto& mv: moves){
                result += std::string(" ") + mv.ToString();
            }
            result += " branch depth: " + std::to_string(BranchDepth());
            result += " local depth: " + std::to_string(local_depth);
            return result;
        }
    };

    class SquareObserver;
    class VariationManager;
    class BaseEvaluator{
    public:
        struct Data{
            EvaluationReport result;
            SquareObserver& obs;
            VariationManager& mgr;
            Data(SquareObserver& obs, VariationManager& mgr)
                    :result(), obs(obs), mgr(mgr)
            {}
            Data(Data& parent)
                    :result(), obs(parent.obs), mgr(parent.mgr)
            {}
        };
        virtual bool operator()(Data& links, Piece own) const = 0;
    };
}

#endif //XO_BASEEVALUATOR_H
