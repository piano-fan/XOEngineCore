#ifndef XO_FIELD_H
#define XO_FIELD_H

#include "types.h"
#include "fieldmetrics.h"


namespace XO {
    class FieldMetrics;

    class Field{
        std::vector<Piece> m_squares;
        const FieldMetrics* m_metrics;
    public:
        void NewGame(){
            m_squares.assign(m_squares.size(), EMPTY);
        }
        void Resize(const FieldMetrics* metrics){
            m_squares.clear();
            m_squares.resize(metrics->GetSquareCount(), EMPTY);
            m_metrics = metrics;
        }

        const Piece& operator[](Point t) const{
            return m_squares[t.GetID()];
        }

        Piece& operator[](Point t){
            return m_squares[t.GetID()];
        }

        std::string ToString() const{
            int x = 0;
            std::string message = "";
            for (Piece p: m_squares) {
                message += PieceName[p];

                x++;
                if (x >= m_metrics->GetWidth()) {
                    x = 0;
                    message += '\n';
                }
            }
            return message;
        }
    };
}

#endif //XO_FIELD_H
