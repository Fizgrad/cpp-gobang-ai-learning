//
// Created by Fiz on 4/30/2021.
//

#ifndef GOBANG_COORDINATE_H
#define GOBANG_COORDINATE_H

class coordinate {
public:
    int x, y;

    coordinate(int x = 0, int y = 0) : x(x), y(y) {}
    coordinate(const coordinate& p): x(p.x), y(p.y){}
};

#endif //GOBANG_COORDINATE_H
