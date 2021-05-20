//
// Created by Fiz on 4/30/2021.
//

#include "gobangAI.h"
#include <thread>
#include <mutex>


namespace {
    std::mutex m;
    std::mutex k;
}

gobangAI::gobangAI() {
}

void gobangAI::clearTheCache(int rounds) {
    this->cache[rounds].clear();
    std::cerr << "clear Zobrist cache " << rounds << std::endl;
}

void gobangAI::initWorthy(bool (&worthyCalculating)[SIZE][SIZE]) {
    for (auto &i:worthyCalculating) {
        for (auto &j:i)
            j = false;
    }
}

void gobangAI::checkWorthyCalculating(const board& b, bool(&worthyCalculating)[SIZE][SIZE]) {
    initWorthy(worthyCalculating);
    for (int i = 0; i < SIZE; ++i)
        for (int j = 0; j < SIZE; ++j)
            if (b.getValue({ i, j }) == WHITE || b.getValue({ i, j }) == BLACK) {
                if (b.getRounds() > 3) {
                    for (int k = -delta; k <= delta; ++k)
                        for (int l = -delta; l <= delta; ++l)
                            if (i + k < SIZE && i + k >= 0 && j + l < SIZE && j + l >= 0 &&
                                b.getValue({ i + k, j + l }) == SPACE)
                                worthyCalculating[i + k][j + l] = true;
                }
                else {
                    for (int k = -1; k <= 1; ++k)
                        for (int l = -1; l <= 1; ++l)
                            if (i + k < SIZE && i + k >= 0 && j + l < SIZE && j + l >= 0 &&
                                b.getValue({ i + k, j + l }) == SPACE)
                                worthyCalculating[i + k][j + l] = true;
                }
            }
}

struct aguments {
    gobangAI *ai;
    board b;
    int deepth;
    int *alpha;
    int *beta;
    const coordinate &i;

    aguments(gobangAI *ai, board &b, int deepth, int *alpha, int *beta, const coordinate &i) : ai(ai), b(b),
                                                                                               alpha(alpha), beta(beta),
                                                                                               i(i), deepth(deepth) {}

    aguments(aguments &a) : ai(a.ai), b(a.b), deepth(a.deepth), alpha(a.alpha), beta(a.beta), i(a.i) {}
};

void procOfWhite(aguments *a) {

    int result;
    a->b.place(a->i);
    board bb(a->b);
    result = a->ai->findMin(bb, a->deepth - 1, *a->alpha, *a->beta);
    a->b.unplace(a->i);
    m.lock();
    if (result > *(a->alpha)) {
        std::cerr << "Best changes from " << a->ai->best.x << " " << a->ai->best.y << " " << *a->beta << " to "
                  << a->i.x << " "
                  << a->i.y << " " << result << std::endl;
        *a->alpha = result;
        a->ai->best = a->i;
    }
    m.unlock();
    delete a;
}


void procOfBlack(aguments *a) {
    int result;
    a->b.place(a->i);
    board bb(a->b);
    result = a->ai->findMax(bb, a->deepth - 1, *a->alpha, *a->beta);
    a->b.unplace(a->i);
    m.lock();
    if (result < *a->beta) {
        std::cerr << "Best changes from " << a->ai->best.x << " " << a->ai->best.y << " " << *a->beta << " to "
                  << a->i.x << " "
                  << a->i.y << " " << result << std::endl;

        *a->beta = result;
        a->ai->best = a->i;
    }
    m.unlock();
    delete a;
}

//black min, white max
int gobangAI::findTheBest(board &b, int deepth, int role) {
    std::vector<std::thread> threads;
    clearTheCache(b.getRounds());
    bool worthyCalculating[SIZE][SIZE];
    initWorthy(worthyCalculating);
    bool isFindKillRole = b.findKill(worthyCalculating, role);
    std::vector<coordinate> positions;
    if (isFindKillRole) {
    } else {
        initWorthy(worthyCalculating);
        if (!b.findKill(worthyCalculating, -role))
            checkWorthyCalculating(b, worthyCalculating);
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
            //std::cerr << "Establish a thread: " << b.getRounds() << std::endl;
            threads.emplace_back(procOfBlack, new aguments(this, b, deepth - 1, &alpha, &beta, i));
        }
    } else if (role == WHITE) {
        for (auto &i : positions) {
            //std::cerr << "Establish a thread: " << b.getRounds() << std::endl;
            threads.emplace_back(procOfWhite, new aguments(this, b, deepth - 1, &alpha, &beta, i));
        }
    }
    std::cerr << "Computing" << std::endl;
    if (!threads.empty())
        for (auto &i : threads) {
            if (i.joinable())
                i.join();
        }
    std::cerr << "Complete computing: " << b.getRounds() + 1 << std::endl;
    return 0;
}

int gobangAI::findMax(board &b, int deepth, int alpha, int beta) {

    if (deepth <= 0 || b.isEnd()) return b.evaluateOverall(WHITE);
    bool worthyCalculating[SIZE][SIZE];
    initWorthy(worthyCalculating);
    bool isFindKill = b.findKill(worthyCalculating, WHITE);
    if (isFindKill) {
    } else {
        if (!b.findKill(worthyCalculating, BLACK))
            checkWorthyCalculating(b, worthyCalculating);
    }
    for (int i = 0; i < SIZE; ++i)
        for (int j = 0; j < SIZE; ++j)
            if (worthyCalculating[i][j]) {
                b.place({i, j});
                int temp;
                int keyValue = b.getKey();
                if (cache[b.getRounds()].find(keyValue) == cache[b.getRounds()].end()) {
                    temp = findMin(b, deepth - 1, alpha, beta);
                    k.lock();
                    cache[b.getRounds()].insert(std::pair<int, int>(keyValue, temp));
                    k.unlock();
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
    if (deepth <= 0 || b.isEnd()) return b.evaluateOverall(BLACK);
    bool worthyCalculating[SIZE][SIZE];
    initWorthy(worthyCalculating);
    bool isFindKill = b.findKill(worthyCalculating, BLACK);
    if (isFindKill) {
    } else {
        if (!b.findKill(worthyCalculating, WHITE))checkWorthyCalculating(b, worthyCalculating);
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
                    k.lock();
                    cache[b.getRounds()].insert(std::pair<int, int>(keyValue, temp));
                    k.unlock();
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