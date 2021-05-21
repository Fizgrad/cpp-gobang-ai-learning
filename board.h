#ifndef BOARD_H
#define BOARD_H


#include "coordinate.h"
#include <iostream>
const int SIZE = 15;
const int BLACK = -1;
const int WHITE = 1;
const int SPACE = 0;
const int EDGE = 2;

const int dx[4] = {1, 0, 1, 1};
const int dy[4] = {0, 1, 1, -1};

class board {
private:
    int boards[SIZE][SIZE];
    int turns = 0;
    int winner = SPACE;
    int hash;

public:

    static int Zobrist[SIZE][SIZE][3];

    static void initZobrist();

    int getKey() const;

    board();

    int getRounds() const;

    board(const board&);

    void initBoards();

    inline void initCheckedList(bool (&checked)[SIZE][SIZE][4]);

    inline bool setCheckedTrue(const coordinate &p, int direction, bool(&checked)[SIZE][SIZE][4]);

    bool isEnd();

    board& place(const coordinate &p);

    board& unplace(const coordinate &p);

    int getTurns()const;

    int getValue(const coordinate &p)const;

    int testEnd();

    void display();

    void logs(const std::string &filename = "out.txt");

    int evaluateOverall(int role);

    int evaluate(int role);

    bool findKill(bool [SIZE][SIZE], int role);

    int getWinner();

};



#endif // BOARD_H
