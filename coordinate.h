#ifndef COORDINATE_H
#define COORDINATE_H
#include<QPoint>
class coordinate
{
public:
    int x, y;
    coordinate(int x = 0, int y = 0);
    coordinate(const coordinate& p);
    coordinate& operator=(const coordinate& i);
    coordinate(const QPoint& p);
};

#endif // COORDINATE_H
