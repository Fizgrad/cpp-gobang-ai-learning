#include "coordinate.h"

coordinate::coordinate(int x , int y ) : x(x), y(y) {}
coordinate::coordinate(const coordinate& p): x(p.x), y(p.y){}

coordinate &coordinate::operator=(const coordinate &i)
{
    this->x=i.x;
    this->y=i.y;
    return *this;
}

coordinate::coordinate(const QPoint &p)
{
    this->x=p.x();
    this->y=p.y();
}

