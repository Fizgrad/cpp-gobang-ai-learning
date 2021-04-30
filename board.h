//
// Created by Fiz on 4/30/2021.
//

#ifndef GOBANG_BOARD_H
#define GOBANG_BOARD_H
#include <iostream>
#include "coordinate.h"

const int SIZE = 15;
const int BLACK = -1;
const int WHITE = 1;
const int SPACE = 0;
const int EDGE = 2;//represent access an invalid grid

const int dx[4] = {1, 1, 0, -1};
const int dy[4] = {0, 1, 1, 1};

class board {
private:
    int boards[SIZE][SIZE];
    int turns = 0;
    int winner = SPACE;
    bool checked[SIZE][SIZE][4];

public:
    board();

    board(const board&);

    void initBoards();

    inline void initCheckedList();

    inline bool setCheckedTrue(const coordinate &p, int direction);

    bool isEnd() const;

    bool place(const coordinate &p);

    int getTurns()const;

    int getValue(const coordinate &p);

    int testEnd();

    void display();

    void logs(const std::string &filename = "out.txt");

    //evaluate unfinished

    int evaluateOverall();

    int evaluate(int role);

};

#endif //GOBANG_BOARD_H
