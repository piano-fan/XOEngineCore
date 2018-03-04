#ifndef XO_DIRECTION_H
#define XO_DIRECTION_H


namespace XO{
    namespace Direction{
        int GetX(int dir);
        int GetY(int dir);
        int IDtoDelta(int ID);
        void DirIDtoXY(int &r_x, int &r_y, int dir, int ID);
    }
}

#endif //XO_DIRECTION_H
