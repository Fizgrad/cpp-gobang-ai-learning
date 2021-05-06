//
// Created by Fiz on 4/30/2021.
//

#include "gobangAI.h"

gobangAI::gobangAI():worthyCalculating(){
    initWorthy();
}

void gobangAI::clearTheCache(int rounds) {
    this->cache.erase(rounds);
    std::clog << "clear Zobrist cache " << rounds << std::endl;
}

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
    if (b.getRounds() > 3) {
        for (int k = -delta; k <= delta; ++k)
            for (int l = -delta; l <= delta; ++l)
                setWorthyCalculatingValueAtPoint(b, {p.x + k, p.y + l});
    } else {
        for (int k = -1; k <= 1; ++k)
            for (int l = -1; l <= 1; ++l)
                setWorthyCalculatingValueAtPoint(b, {p.x + k, p.y + l});
    }
}

void gobangAI::setWorthyCalculatingValueAtPoint(const board &b, const coordinate &p) {
    if (p.x < SIZE && p.x >= 0 && p.y < SIZE && p.y >= 0 && b.getValue(p) == SPACE)
        worthyCalculating[p.x][p.y] = true;
}

//black min, white max
int gobangAI::findTheBest(board &b, int deepth, int role) {
    clearTheCache(b.getRounds());
    initWorthy();
    bool isFindKillRole =b.findKill(worthyCalculating,role);
    std::vector<coordinate> positions;
    if(isFindKillRole){
    }
    else{
        initWorthy();
        if(!b.findKill(worthyCalculating,-role))
        checkWorthyCalculating(b);
    }

    for (int i = 0; i < SIZE; ++i)
        for (int j = 0; j < SIZE; ++j)
            if (worthyCalculating[i][j])
                positions.emplace_back(i, j);
    if (positions.empty()) {
        this->best = coordinate((SIZE - 1) / 2, (SIZE - 1) / 2);
        return 0;
    }
    int alpha = INT32_MIN;
    int beta = INT32_MAX;
    if (role == BLACK) {
        for (auto &i : positions) {
            b.place(i);
            int temp = findMax(b, deepth - 1, alpha, beta);
            b.unplace(i);
            if (temp < beta) {
                std::clog << "Best changes from " << best.x << " " << best.y << " " << beta << " to " << i.x << " "
                          << i.y << " " << temp << std::endl;
                beta = temp;
                this->best = i;
            }
        }
    } else if (role == WHITE) {
        for (auto &i : positions) {
            b.place(i);
            int temp = findMin(b, deepth - 1, alpha, beta);
            b.unplace(i);
            if (temp > alpha) {
                std::clog << "Best changes from " << best.x << " " << best.y << " " << alpha << " to " << i.x << " "
                          << i.y << " " << temp << std::endl;
                alpha = temp;
                this->best = i;
            }
        }
    }
    return 0;
}


int gobangAI::findMax(board &b, int deepth, int alpha, int beta) {

    if (deepth <= 0 || b.isEnd()) return b.evaluateOverall(BLACK);
    initWorthy();


    bool isFindKill =b.findKill(worthyCalculating,WHITE);
    if(isFindKill){
    }
    else {
        if(!b.findKill(worthyCalculating,BLACK));
        checkWorthyCalculating(b);
    }
    for (int i = 0; i < SIZE; ++i)
        for (int j = 0; j < SIZE; ++j)
            if (worthyCalculating[i][j]) {
                b.place({i, j});
                int temp;
                int keyValue = b.getKey();
                if (cache[b.getRounds()].find(keyValue) == cache[b.getRounds()].end()) {
                    temp = findMin(b, deepth - 1, alpha, beta);
                    cache[b.getRounds()].insert(std::pair<int, int>(keyValue, temp));
                } else {
                    temp = cache[b.getRounds()][keyValue];
                }
                b.unplace({i, j});
                if (temp > alpha) {
                    alpha = temp;
                    if (alpha >= beta) {
                        return alpha;
                    }
                }

            }
    return alpha;
}

int gobangAI::findMin(board &b, int deepth, int alpha, int beta) {
    if (deepth <= 0 || b.isEnd()) return b.evaluateOverall(WHITE);
    initWorthy();
    bool isFindKill =b.findKill(worthyCalculating,BLACK);
    if(isFindKill){
    }
    else {
        if(!b.findKill(worthyCalculating,WHITE))checkWorthyCalculating(b);
    }
    //b.logs("log.txt");
    for (int i = 0; i < SIZE; ++i)
        for (int j = 0; j < SIZE; ++j)
            if (worthyCalculating[i][j]) {
                b.place({i, j});
                int temp;
                int keyValue = b.getKey();
                if (cache[b.getRounds()].find(keyValue) == cache[b.getRounds()].end()) {
                    temp = findMax(b, deepth - 1, alpha, beta);
                    cache[b.getRounds()].insert(std::pair<int, int>(keyValue, temp));
                } else {
                    temp = cache[b.getRounds()][keyValue];
                }
                b.unplace({i, j});
                if (temp < beta) {
                    beta = temp;
                    if (alpha >= beta) {
                        return beta;
                    }
                }
            }
    return beta;
}