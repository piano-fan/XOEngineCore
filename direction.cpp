#include "direction.h"


namespace XO{
    namespace Direction {
        const int m_X[] = {1, 1, 0, -1};
        const int m_Y[] = {0, 1, 1, 1};

        int GetX(int dir) {
            return m_X[dir];
        }

        int GetY(int dir) {
            return m_Y[dir];
        }

        int IDtoDelta(int ID){
            return ID < 4 ? ID - 4 : ID - 3;
        }

        void DirIDtoXY(int &r_x, int &r_y, int dir, int ID){
            r_x = GetX(dir) * IDtoDelta(ID);
            r_y = GetY(dir) * IDtoDelta(ID);
        }
    }
}