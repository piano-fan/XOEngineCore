#ifndef XO_BASEEVALUATOR_H
#define XO_BASEEVALUATOR_H

#include "types.h"


namespace XO{
    struct EvaluationReport{
        enum class Mode{
            UNDEFINED = 0, STATIC_EVALUATION = 1
            , STATIC_TACTICS = 2, DEPRECATED_TACTICS = 3
            , DEEP_TACTICS = 4
        };
        enum class Type{
            NONE = 0, SUCCESS = 1, FAIL = 2, DEPTH_LIMIT = 3
        };
        static constexpr const char* evaluation_name[] = {
                "Undefined", "Static evaluation", "Static tactics", "Deprecated", "Deep tactics"
        };
        static constexpr const char* type_name[] = {
                "None", "Success", "Fail", "Depth limit"
        };
        std::list<Move> moves;
        DepthT local_depth;
        Mode mode;
        Type type;

        EvaluationReport()
                :moves(), local_depth(0), mode(Mode::UNDEFINED), type(Type::NONE)
        {}

        EvaluationReport(DepthT local_depth, Mode mode, Type type)
                :moves(), local_depth(local_depth), mode(mode), type(type)
        {}

        EvaluationReport(const Move& mv, DepthT local_depth, Mode mode, Type type)
                :moves(), local_depth(local_depth), mode(mode), type(type)
        {
            moves.push_back(mv);
        }

        void Clear(){
            *this = EvaluationReport();
        }

        EvaluationReport& operator=(const EvaluationReport& src) = default;

        EvaluationReport& operator=(EvaluationReport&& src){
            moves = std::move(src.moves);
            local_depth = src.local_depth;
            mode = src.mode;
            type = src.type;
            return *this;
        }

        DepthT BranchDepth() const{
            return moves.size();
        }

        DepthT FullDepth() const{
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

        bool OldDepthReport(DepthT current_depth_limit) const{
            return DepthLimit()
                && local_depth != current_depth_limit;
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
            const SquareObserver& obs;
            VariationManager& mgr;
            Data(const SquareObserver& obs, VariationManager& mgr)
                    :result(), obs(obs), mgr(mgr)
            {}
            Data(Data& parent)
                    :result(), obs(parent.obs), mgr(parent.mgr)
            {}
        };
        virtual void operator()(Data& links, Piece own) const = 0;
    };

    template<typename EvAgent>
    class P_Final{
    public:
        bool operator()(BaseEvaluator::Data& links, Piece own) const{
            EvAgent()(links, own);
            return links.result.Final();
        }
    };
}

#endif //XO_BASEEVALUATOR_H
