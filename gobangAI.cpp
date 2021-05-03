//
// Created by Fiz on 4/30/2021.
//

#include "gobangAI.h"
#include <vector>


void gobangAI::initWorthy() {
    for (auto &i:worthyCalculating) {
        for (auto &j:i)
            j = false;
    }
}

void gobangAI::checkWorthyCalculating(const board &b) {
    initWorthy();
    for (int i = 0; i < SIZE; ++i)
        for (int j = 0; j < SIZE; ++j)
            if (b.getValue({i, j}) == WHITE || b.getValue({i, j}) == BLACK)
                setWorthyCalculatingAroundPoint(b, {i, j});
}

void gobangAI::setWorthyCalculatingAroundPoint(const board &b, const coordinate &p) {
    for (int k = -delta; k <= delta; ++k)
        for (int l = -delta; l <= delta; ++l)
            setWorthyCalculatingValueAtPoint(b, {p.x + k, p.y + l});
}

void gobangAI::setWorthyCalculatingValueAtPoint(const board &b, const coordinate &p) {
    if (p.x < SIZE && p.x >= 0 && p.y < SIZE && p.y >= 0 && b.getValue(p) == SPACE)
        worthyCalculating[p.x][p.y] = true;
}


//multiplying black or white with evaluated results will make this function goal become to find max
//black min, white max
int gobangAI::findTheBest(board& b, int deepth, int role) {
    std::vector<coordinate> positions;
    checkWorthyCalculating(b);
    for (int i = 0; i < SIZE; ++i)
        for (int j = 0; j < SIZE; ++j)
            if (worthyCalculating[i][j])
                positions.emplace_back(i, j);
    if (positions.empty()) {
        this->best = coordinate((SIZE - 1) / 2, (SIZE - 1) / 2);
        return 0;
    }
    int result ;
    b.logs("log.txt");
    int temp;
    if(role == BLACK)
    {
        result = INT32_MAX;
        for(auto& i : positions) {
            b.place(i);
            temp = findMax(b, deepth - 1);
            b.unplace(i);
            if (temp < result) {
                std::cout<<"Best changes from "<<best.x<<" "<<best.y<<" " <<result<<" to "<<i.x<<" "<<i.y<<" "<<temp<<std::endl;
                result = temp;
                this->best = i;
            }
        }
    }
    else if(role == WHITE){
        result = INT32_MIN;
        for(auto& i : positions)
        {
            b.place(i);
            temp = findMin(b,deepth-1);
            b.unplace(i);
            if(temp>result)
            {
                std::cout<<"Best changes from "<<best.x<<" "<<best.y<<" " <<result<<" to "<<i.x<<" "<<i.y<<" "<<temp<<std::endl;
                result = temp;
                this->best = i;
            }
        }
    }
}

int gobangAI::findMax(board& b, int deepth) {
    if (deepth <= 0 || b.isEnd())
        return b.evaluateOverall();
    deepth--;
    checkWorthyCalculating(b);
    b.logs("log.txt");
    int result = INT32_MIN;
    for (int i = 0; i < SIZE; ++i)
        for (int j = 0; j < SIZE; ++j)
            if (worthyCalculating[i][j]) {
                int temp = findMin(b.place({i, j}), deepth);
                b.unplace({i,j});
                if(temp>result)
                {
                    result = temp;
                }
            }
    return result;
}

int gobangAI::findMin(board& b, int deepth) {
    if (deepth <= 0 || b.isEnd()) {
        return b.evaluateOverall();
    }
    deepth--;
    checkWorthyCalculating(b);
    b.logs("log.txt");
    int result =INT32_MAX;
    for (int i = 0; i < SIZE; ++i)
        for (int j = 0; j < SIZE; ++j)
            if (worthyCalculating[i][j]) {
                int temp = findMax(b.place({i,j}), deepth);
                b.unplace({i,j});
                if (temp < result)
                {
                    result = temp;
                }
            }
    return result;
}