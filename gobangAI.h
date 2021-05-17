//
// Created by Fiz on 4/30/2021.
//

#ifndef GOBANG_GOBANGAI_H
#define GOBANG_GOBANGAI_H

#include "coordinate.h"
#include "board.h"
#include <vector>
#include <map>
const int delta = 2;

class gobangAI {
private:
    std::map<int,std::map<int, int>> cache; // cache storing the previous results

public:
    coordinate best; //the best step

    void setWorthyCalculatingValueAtPoint(const board &b, const coordinate &p,bool (&worthyCalculating)[SIZE][SIZE]);

    gobangAI();

    void clearTheCache(int);

    int findTheBest(board&, int deepth, int role);

    int findMax(board& ,int deepth,int alpha, int beta);

    int findMin(board& ,int deepth,int alpha, int beta);

    void checkWorthyCalculating(const board &,bool (&worthyCalculating)[SIZE][SIZE]);

    void setWorthyCalculatingAroundPoint(const board&,const coordinate &,bool (&worthyCalculating)[SIZE][SIZE]);

    void initWorthy(bool (&worthyCalculating)[SIZE][SIZE]);
};

#endif //GOBANG_GOBANGAI_H
