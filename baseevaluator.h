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
            NONE = 0, SUCCESS = 1, FAIL = 2
        };
        static constexpr const char* evaluation_name[] = {
                "Undefined", "Static tactics", "Deep tactics", "Static evaluation", "Deprecated"
        };
        static constexpr const char* type_name[] = {
                "None", "Success", "Fail"
        };
        Point point;
        DValueT local_depth;
        Mode mode;
        Type type;

        EvaluationReport()
                :point(), local_depth(0), mode(Mode::UNDEFINED), type(Type::NONE)
        {}

        EvaluationReport(const Move& mv, DValueT local_depth, Mode mode, Type type)
                :point(mv.GetPos()), local_depth(local_depth), mode(mode), type(type)
        {}

        void Clear(){
            new (this) EvaluationReport();
        }

        DValueT BranchDepth() const{
            return 0;
        }

        DValueT FullDepth() const{
            return BranchDepth() + local_depth;
        }

        std::string ToString() const{
            std::string result = evaluation_name[(int)mode];
            result += ": ";
            result += type_name[(int)type];
            result += std::string(" ") + point.ToString();
            result += " branch depth: " + std::to_string(BranchDepth());
            result += " local depth: " + std::to_string(local_depth);
            return result;
        }
    };

    class SquareObserver;
    class BaseEvaluator{
    public:
        struct Data{
            EvaluationReport result;
            SquareObserver& obs;
            Data(SquareObserver& obs)
                    :result(), obs(obs)
            {}
        };
        virtual bool operator()(Data& links, Piece own) const = 0;
    };
}

#endif //XO_BASEEVALUATOR_H
