#ifndef REPORT_H
#define REPORT_H

#include "types.h"


namespace XO{
    struct Report{
        enum Author{
            UNKNOWN = 0, STATIC_EVALUATION = 1, STATIC_TACTICS = 2, DEPRECATED_TACTICS = 3
        };

        enum Type{
            UNTOUCHED = 0, NONE = 1, SUCCESS = 2, FAIL = 3, DEPTH_LIMIT = 4
        };

        Author author;
        Type type;
        Move move;
        DepthT depth;

        Report()
            :author(), type(), move(), depth()
        {}

        Report(Author a, Type t, Move m, DepthT d)
            :author(a), type(t), move(m), depth(d)
        {}

        operator bool() const{
            return type != UNTOUCHED;
        }

        bool Final() const{
            return type != UNTOUCHED
                && type != DEPTH_LIMIT
                && type != NONE;
        }

        bool Success() const{
            return type == SUCCESS;
        }

        bool DepthLimit() const{
            return type == DEPTH_LIMIT;
        }

        bool None() const{
            return type == NONE;
        }

        bool Fail() const{
            return type == FAIL;
        }

        std::string ToString() const{
            static constexpr const char* AuthorName[] = {
                    "Unknown", "Static evaluation", "Static tactics", "Deprecated tactics"
            };
            static constexpr const char* TypeName[] = {
                    "Untouched", "None", "Success", "Fail", "Depth limit"
            };
            return std::string() + AuthorName[author] + ": "
                    + TypeName[type] + " "
                    + move.ToString()
                    + ( depth ? " Depth: " + std::to_string((int)depth) : "" );
        }
    };
}

#endif //REPORT_H
