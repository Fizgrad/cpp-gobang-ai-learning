//
// Created by Fiz on 4/30/2021.
//

#ifndef GOBANG_GOBANGAI_H
#define GOBANG_GOBANGAI_H

#include "coordinate.h"
#include "board.h"
#include <vector>

const int delta = 2;

class gobangAI {
private:
    bool worthyCalculating[SIZE][SIZE];//used for local search

public:

    coordinate best;

    int findTheBest(board&, int deepth, int role);

    int findMax(board& ,int deepth);

    int findMin(board& ,int deepth);

    void checkWorthyCalculating(const board &);

    void setWorthyCalculatingValueAtPoint(const board&,const coordinate &);

    void setWorthyCalculatingAroundPoint(const board&,const coordinate &);

    void initWorthy();
};


#endif //GOBANG_GOBANGAI_H
