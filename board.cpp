//
// Created by Fiz on 4/30/2021.
//

#include "coordinate.h"
#include "board.h"
#include <iostream>
#include <fstream>
#include <random>
#include <ctime>
#include <mutex>

namespace {
    std::mutex l;
}

int board::Zobrist[SIZE][SIZE][3] = {};

int board::getWinner() {
    return this->winner;
}

int board::getTurns() const {
    return this->turns % 2 ? WHITE : BLACK;
}

int board::getRounds() const {
    return this->turns;
}

int board::getKey() const{
    return this->hash;
}

board::board():boards(),hash(0){
    initBoards();
    turns = 0;
    winner = SPACE;
    for(int i = 0;i<SIZE;++i)
    {
        for(int j = 0;j<SIZE;++j)
            hash ^=Zobrist[i][j][boards[i][j]+1];
    }
}

board::board(const board &b) : boards(),hash(b.getKey()){
    this->turns = b.turns;
    this->winner = b.winner;
    for (int i = 0; i < SIZE; ++i)
        for (int j = 0; j < SIZE; ++j)
            this->boards[i][j] = b.boards[i][j];
}

void board::initBoards() {
    for (auto &i : boards)
        for (auto &j : i)
            j = SPACE;
}

inline void board::initCheckedList(bool(&checked)[SIZE][SIZE][4]) {
    for (auto &i : checked)
        for (auto &j : i)
            for (auto &k :j)
                k = false;
}

inline bool board::setCheckedTrue(const coordinate &p, int direction, bool(&checked)[SIZE][SIZE][4]) {
    if (p.x < SIZE && p.x >= 0 && p.y >= 0 && p.y < SIZE)
    {
        checked[p.x][p.y][direction] = true;
        return checked[p.x][p.y][direction];
    }
    else return false;
}

bool board::isEnd() {
    this->testEnd();
    return this->winner != SPACE || this->turns >= SIZE * SIZE;
}

//place a chess at given coordinate. the type depends on turns.

board &board::place(const coordinate &p) {
    int temp = getValue(p);
    if (p.x < SIZE && p.x >= 0 && p.y >= 0 && p.y < SIZE && boards[p.x][p.y] == SPACE) {
        this->boards[p.x][p.y] = (turns % 2 ? WHITE : BLACK);
        hash^=Zobrist[p.x][p.y][temp+1]^Zobrist[p.x][p.y][(turns % 2 ? WHITE : BLACK)+1];
        ++turns;
    } else {
        std::cout << "Invalid coordinate to place" << std::endl;
        l.lock();
        std::ofstream out("error.txt", std::ios::app);
        out << "Try to place " << p.x << "," << p.y << " ! INVALID ! MAP BELOW" << std::endl;
        logs("error.txt");
        l.unlock();
    }
    return *this;
}

board &board::unplace(const coordinate &p) {
    if (p.x < SIZE && p.x >= 0 && p.y >= 0 && p.y < SIZE && boards[p.x][p.y] != SPACE) {
        hash^=Zobrist[p.x][p.y][SPACE+1]^Zobrist[p.x][p.y][boards[p.x][p.y]+1];
        this->boards[p.x][p.y] = SPACE;
        --turns;
    }
    else {
        std::cerr<<"Invalid coordinate to unplace"<<std::endl;
        l.lock();
        std::ofstream out("error.txt", std::ios::app);
        out << "Try to unplace " << p.x << "," << p.y << " ! INVALID!" << std::endl;
        logs("error.txt");
        l.unlock();
    }
    return *this;
}

//to return the value of the chess placed at the given and valid coordinate,if the given coordinate is invalid,
//return the value of EDGE, AKA 2, which doesn't equal to white or black.
int board::getValue(const coordinate &p) const {
    return (p.x >= SIZE || p.y >= SIZE || p.x < 0 || p.y < 0) ? EDGE : this->boards[p.x][p.y];
}

//to check whether someone is winner
int board::testEnd() {
    bool checked[SIZE][SIZE][4];
    initCheckedList(checked);
    for (int i = 0; i < SIZE; ++i) {
        for (int j = 0; j < SIZE; ++j) {
            if (this->boards[i][j] != SPACE)
                for (int k = 0; k < 4; ++k) {
                    if (checked[i][j][k])
                        continue;
                    if (boards[i][j] == getValue({i + dx[k], j + dy[k]}) &&
                        boards[i][j] == getValue({i + 2 * dx[k], j + 2 * dy[k]}) &&
                        boards[i][j] == getValue({i + 3 * dx[k], j + 3 * dy[k]}) &&
                        boards[i][j] == getValue({i + 4 * dx[k], j + 4 * dy[k]})) {
                        //std::cout << "Test end" << std::endl;
                        this->winner = boards[i][j];
                        return this->winner;
                    } else {
                        for (int temp = 0; temp <= 4; ++temp) {
                            if (boards[i][j] == getValue({i + temp * dx[k], j + temp * dy[k]}))
                                setCheckedTrue({i + temp * dx[k], j + temp * dy[k]}, k,checked);
                            else break;
                        }
                    }
                }
        }
    }
    winner = SPACE;
    return SPACE;
}

//print the board
void board::display() {
    using std::cout;
    cout << turns << ": ";
    cout << (turns % 2 ? "B" : "W") << std::endl;
    cout << "  ";
    for (int i = 0; i < SIZE; ++i)
        cout << char(i < 10 ? i + '0' : 'A' + i - 10);
    cout << std::endl;
    for (int i = 0; i < SIZE; ++i) {
        cout << char(i < 10 ? i + '0' : 'A' + i - 10) << " ";
        for (int j = 0; j < SIZE; ++j)
            switch (boards[i][j]) {
                case BLACK:
                    cout << '@';
                    break;
                case WHITE:
                    cout << 'O';
                    break;
                case SPACE:
                    cout << ' ';
                    break;
                default:
                    break;
            }
        cout << '\n';
    }
    cout << "Evaluate: B " << evaluate(BLACK) << " W " << evaluate(WHITE) << std::endl << std::endl;
    if (winner != SPACE)
        cout << "winner is " << winner << std::endl;
}

//print the information of each step in a file
void board::logs(const std::string &filename) {
    std::ofstream out(filename, std::ios::app);
    out << turns << ": ";
    out << (turns % 2 ? "B" : "W") << std::endl;
    out << "  ";
    for (int i = 0; i < SIZE; ++i)
        out << char(i < 10 ? i + '0' : 'A' + i - 10);
    out << std::endl;
    for (int i = 0; i < SIZE; ++i) {
        out << char(i < 10 ? i + '0' : 'A' + i - 10) << " ";
        for (int j = 0; j < SIZE; ++j)
            switch (boards[i][j]) {
                case BLACK:
                    out << '@';
                    break;
                case WHITE:
                    out << 'O';
                    break;
                case SPACE:
                    out << ' ';
                    break;
                default:
                    break;
            }
        out << '\n';
    }
    out << "Evaluate: B " << evaluate(BLACK) << " W " << evaluate(WHITE) << std::endl << std::endl;
    if (winner != SPACE)
        out << "winner is " << winner << std::endl;
}

//analyze the whole situation
int board::evaluateOverall(int role) {
    int result = evaluate(role) + evaluate(-role)+ (evaluate(-role)>>3);
    return result;
}

//white more is better, black less is better
int board::evaluate(int role) {
    bool checked[SIZE][SIZE][4];
    initCheckedList(checked);
    int result = 0;
    for (int i = 0; i < SIZE; ++i) {
        for (int j = 0; j < SIZE; ++j) {
            if (this->boards[i][j] == role)
                for (int k = 0; k < 4; ++k) {
                    if (checked[i][j][k])
                        continue;
                    else {
                        // 1 1 1 1 1
                        if (boards[i][j] == getValue({i + dx[k], j + dy[k]}) &&
                            boards[i][j] == getValue({i + 2 * dx[k], j + 2 * dy[k]}) &&
                            boards[i][j] == getValue({i + 3 * dx[k], j + 3 * dy[k]}) &&
                            boards[i][j] == getValue({i + 4 * dx[k], j + 4 * dy[k]})) {
                            return role*(INT32_MAX>>1);
                        }
                            // 0 1 1 1 1 0
                        else if (boards[i][j] == getValue({i + dx[k], j + dy[k]}) &&
                                 boards[i][j] == getValue({i + 2 * dx[k], j + 2 * dy[k]}) &&
                                 boards[i][j] == getValue({i + 3 * dx[k], j + 3 * dy[k]}) &&
                                 getValue({i + 4 * dx[k], j + 4 * dy[k]}) == SPACE &&
                                 getValue({i - dx[k], j - dy[k]}) == SPACE) {
                            result += getValue({i, j}) * (1000000);
                            for (int temp = 0; temp <= 3; ++temp)
                                setCheckedTrue({i + temp * dx[k], j + dy[k] * temp}, k, checked);
                        }
                            // 0 1 1 1 1 # or # 1 1 1 1 0
                        else if (boards[i][j] == getValue({i + dx[k], j + dy[k]}) &&
                                 boards[i][j] == getValue({i + 2 * dx[k], j + 2 * dy[k]}) &&
                                 boards[i][j] == getValue({i + 3 * dx[k], j + 3 * dy[k]}) &&
                                 (getValue({i + 4 * dx[k], j + 4 * dy[k]}) == SPACE ||
                                  getValue({i - dx[k], j - dy[k]}) == SPACE)) {
                            result += getValue({i, j}) * (500000);
                            for (int temp = 0; temp <= 3; ++temp)
                                setCheckedTrue({i + temp * dx[k], j + dy[k] * temp}, k, checked);
                        }
                            // # 1 1 1 1 #
                        else if (boards[i][j] == getValue({i + dx[k], j + dy[k]}) &&
                                 boards[i][j] == getValue({i + 2 * dx[k], j + 2 * dy[k]}) &&
                                 boards[i][j] == getValue({i + 3 * dx[k], j + 3 * dy[k]})) {
                            result += getValue({i, j}) * (1000);
                            for (int temp = 0; temp <= 3; ++temp)
                                setCheckedTrue({i + temp * dx[k], j + dy[k] * temp}, k, checked);
                        }
                            // 1 1 1 0 1
                        else if (boards[i][j] == getValue({i + dx[k], j + dy[k]}) &&
                                 boards[i][j] == getValue({i + 2 * dx[k], j + 2 * dy[k]}) &&
                                 getValue({i + 3 * dx[k], j + 3 * dy[k]}) == SPACE &&
                                 boards[i][j] == getValue({i + 4 * dx[k], j + 4 * dy[k]})) {
                            result += getValue({i, j}) * (405000);
                            for (int temp = 0; temp <= 4; ++temp)
                                setCheckedTrue({i + temp * dx[k], j + dy[k] * temp}, k, checked);
                        }
                            // 1 1 0 1 1
                        else if (boards[i][j] == getValue({i + dx[k], j + dy[k]}) &&
                                 boards[i][j] == getValue({i + 3 * dx[k], j + 3 * dy[k]}) &&
                                 getValue({i + 2 * dx[k], j + 2 * dy[k]}) == SPACE &&
                                 boards[i][j] == getValue({i + 4 * dx[k], j + 4 * dy[k]})) {
                            result += getValue({i, j}) * (400000);
                            for (int temp = 0; temp <= 4; ++temp)
                                setCheckedTrue({i + temp * dx[k], j + dy[k] * temp}, k, checked);
                        }
                            // 1 0 1 1 1
                        else if (getValue({i, j}) == getValue({i + 2 * dx[k], j + 2 * dy[k]}) &&
                                 getValue({i, j}) == getValue({i + 3 * dx[k], j + 3 * dy[k]}) &&
                                 getValue({i, j}) == getValue({i + 4 * dx[k], j + 4 * dy[k]}) &&
                                 getValue({i + dx[k], j + dy[k]}) == SPACE) {
                            result += getValue({i, j}) * (405000);
                            for (int temp = 0; temp <= 4; ++temp)
                                setCheckedTrue({i + temp * dx[k], j + dy[k] * temp}, k, checked);
                        }
                            // 0 1 0 1 1 0
                        else if (getValue({i, j}) == getValue({i + 2 * dx[k], j + 2 * dy[k]}) &&
                                 getValue({i, j}) == getValue({i + 3 * dx[k], j + 3 * dy[k]}) &&
                                 getValue({i + 4 * dx[k], j + 4 * dy[k]}) == SPACE &&
                                 getValue({i + dx[k], j + dy[k]}) == SPACE &&
                                 getValue({i - dx[k], j - dy[k]}) == SPACE) {
                            result += getValue({i, j}) * (40000);
                            for (int temp = 0; temp <= 3; ++temp)
                                setCheckedTrue({i + temp * dx[k], j + dy[k] * temp}, k, checked);
                        }
                            // 0 1 1 0 1 0
                        else if (getValue({i, j}) == getValue({i + dx[k], j + dy[k]}) &&
                                 getValue({i, j}) == getValue({i + 3 * dx[k], j + 3 * dy[k]}) &&
                                 getValue({i + 4 * dx[k], j + 4 * dy[k]}) == SPACE &&
                                 getValue({i + 2 * dx[k], j + 2 * dy[k]}) == SPACE &&
                                 getValue({i - dx[k], j - dy[k]}) == SPACE) {
                            result += getValue({i, j}) * (40000);
                            for (int temp = 0; temp <= 3; ++temp)
                                setCheckedTrue({i + temp * dx[k], j + dy[k] * temp}, k, checked);
                        }
                            // 0 1 1 1 0
                        else if (getValue({i, j}) == getValue({i + dx[k], j + dy[k]}) &&
                                 getValue({i, j}) == getValue({i + 2 * dx[k], j + 2 * dy[k]}) &&
                                 getValue({i + 3 * dx[k], j + 3 * dy[k]}) == SPACE &&
                                 getValue({i - dx[k], j - dy[k]}) == SPACE) {
                            result += getValue({i, j}) * 50000;
                            for (int temp = 0; temp <= 2; ++temp)
                                setCheckedTrue({i + temp * dx[k], j + dy[k] * temp}, k, checked);
                        }
                            // 1 1 1 0
                        else if (getValue({i, j}) == getValue({i + dx[k], j + dy[k]}) &&
                                 getValue({i, j}) == getValue({i + 2 * dx[k], j + 2 * dy[k]}) &&
                                 getValue({i + 3 * dx[k], j + 3 * dy[k]}) == SPACE) {
                            result += getValue({i, j}) * 1100;
                            for (int temp = 0; temp <= 2; ++temp)
                                setCheckedTrue({i + temp * dx[k], j + dy[k] * temp}, k, checked);
                        }
                            //0 1 1 1
                        else if (getValue({i, j}) == getValue({i + dx[k], j + dy[k]}) &&
                                 getValue({i, j}) == getValue({i + 2 * dx[k], j + 2 * dy[k]}) &&
                                 getValue({i - dx[k], j - dy[k]}) == SPACE
                                ) {
                            result += getValue({i, j}) * 1100;
                            for (int temp = 0; temp <= 2; ++temp)
                                setCheckedTrue({i + temp * dx[k], j + dy[k] * temp}, k, checked);
                        }
                            // 1 1 0 1 0
                        else if (getValue({i, j}) == getValue({i + dx[k], j + dy[k]}) &&
                                 getValue({i, j}) == getValue({i + 3 * dx[k], j + 3 * dy[k]}) &&
                                 getValue({i + 2 * dx[k], j + 2 * dy[k]}) == SPACE &&
                                 getValue({i + 4 * dx[k], j + 4 * dy[k]}) == SPACE) {
                            result += getValue({i, j}) * 900;
                            for (int temp = 0; temp <= 3; ++temp)
                                setCheckedTrue({i + temp * dx[k], j + dy[k] * temp}, k, checked);
                        }
                            // 0 1 1 0
                        else if (getValue({i, j}) == getValue({i + dx[k], j + dy[k]}) &&
                                 getValue({i + 2 * dx[k], j + 2 * dy[k]}) == SPACE &&
                                 getValue({i - dx[k], j - dy[k]}) == SPACE) {
                            result += getValue({i, j}) * (100);
                            //std::cout<<turns<<": "<<i<<" "<<j<<" "<<k<<std::endl;
                            for (int temp = 0; temp <= 1; ++temp)
                                setCheckedTrue({i + temp * dx[k], j + dy[k] * temp}, k, checked);
                        }
                        // 0 1 1 
                        else if (getValue({ i, j }) == getValue({ i + dx[k], j + dy[k] }) &&
                        (getValue({ i + 2 * dx[k], j + 2 * dy[k] }) == SPACE ||
                        getValue({ i - dx[k], j - dy[k] }) == SPACE)) {
                        result += getValue({ i, j }) * (10);
                        //std::cout<<turns<<": "<<i<<" "<<j<<" "<<k<<std::endl;
                        for (int temp = 0; temp <= 1; ++temp)
                            setCheckedTrue({ i + temp * dx[k], j + dy[k] * temp }, k, checked);
                        }
                          
                            // 0 1 0 1 0
                        else if (getValue({i, j}) == getValue({i + 2 * dx[k], j + 2 * dy[k]}) &&
                                 getValue({i + dx[k], j + dy[k]}) == SPACE &&
                                 getValue({i - dx[k], j - dy[k]}) == SPACE &&
                                 getValue({i + 3 * dx[k], j + 3 * dy[k]}) == SPACE) {
                            result += getValue({i, j}) * (5);
                            //std::cout<<turns<<": "<<i<<" "<<j<<" "<<k<<std::endl;
                            for (int temp = 0; temp <= 2; ++temp)
                                setCheckedTrue({i + temp * dx[k], j + dy[k] * temp}, k, checked);
                        }
                        // 0 1 X
                        else if (
                        getValue({ i + dx[k], j + dy[k] }) == -getValue({ i , j}) ||
                        getValue({ i - dx[k], j - dy[k] }) == -getValue({ i , j })) {
                        result += getValue({ i, j }) * (2);
                        //std::cout<<turns<<": "<<i<<" "<<j<<" "<<k<<std::endl;
                        for (int temp = 0; temp <= 0; ++temp)
                            setCheckedTrue({ i + temp * dx[k], j + dy[k] * temp }, k, checked);
                        }
                        // 0 1 0
                        else if (
                        getValue({ i + dx[k], j + dy[k] }) == SPACE &&
                        getValue({ i - dx[k], j - dy[k] }) == SPACE) {
                        result += getValue({ i, j }) * (1);
                        //std::cout<<turns<<": "<<i<<" "<<j<<" "<<k<<std::endl;
                        for (int temp = 0; temp <= 0; ++temp)
                            setCheckedTrue({ i + temp * dx[k], j + dy[k] * temp }, k, checked);
                        }
                    }
                }
        }
    }

    return result;
}



bool board::findKill(bool a[SIZE][SIZE], int role) {
    for (int i = 0; i < SIZE; ++i) {
        for (int j = 0; j < SIZE; ++j) {
            if (this->boards[i][j] == role)
                for (int k = 0; k < 4; ++k) {
                    // 0 1 1 1 1 0
                    if (boards[i][j] == getValue({ i + dx[k], j + dy[k] }) &&
                        boards[i][j] == getValue({ i + 2 * dx[k], j + 2 * dy[k] }) &&
                        boards[i][j] == getValue({ i + 3 * dx[k], j + 3 * dy[k] }) &&
                        getValue({ i + 4 * dx[k], j + 4 * dy[k] }) == SPACE &&
                        getValue({ i - dx[k], j - dy[k] }) == SPACE) {
                        a[i + 4 * dx[k]][j + 4 * dy[k]] = true;
                        a[i - dx[k]][j - dy[k]] = true;
                        return true;
                    }
                    // 0 1 1 1 1 # or # 1 1 1 1 0
                    else if (boards[i][j] == getValue({ i + dx[k], j + dy[k] }) &&
                        boards[i][j] == getValue({ i + 2 * dx[k], j + 2 * dy[k] }) &&
                        boards[i][j] == getValue({ i + 3 * dx[k], j + 3 * dy[k] }) &&
                        (getValue({ i + 4 * dx[k], j + 4 * dy[k] }) == SPACE &&
                            getValue({ i - dx[k], j - dy[k] }) != SPACE)) {
                        a[i + 4 * dx[k]][j + 4 * dy[k]] = true;
                        return true;
                    }
                    else if (boards[i][j] == getValue({ i + dx[k], j + dy[k] }) &&
                        boards[i][j] == getValue({ i + 2 * dx[k], j + 2 * dy[k] }) &&
                        boards[i][j] == getValue({ i + 3 * dx[k], j + 3 * dy[k] }) &&
                        (getValue({ i + 4 * dx[k], j + 4 * dy[k] }) != SPACE &&
                            getValue({ i - dx[k], j - dy[k] }) == SPACE)) {
                        a[i - dx[k]][j - dy[k]] = true;
                        return true;
                    }
                    // 1 1 1 0 1
                    else if (boards[i][j] == getValue({ i + dx[k], j + dy[k] }) &&
                        boards[i][j] == getValue({ i + 2 * dx[k], j + 2 * dy[k] }) &&
                        getValue({ i + 3 * dx[k], j + 3 * dy[k] }) == SPACE &&
                        boards[i][j] == getValue({ i + 4 * dx[k], j + 4 * dy[k] })) {
                        a[i + 3 * dx[k]][j + 3 * dy[k]] = true;
                        return true;
                    }
                    // 1 1 0 1 1
                    else if (boards[i][j] == getValue({ i + dx[k], j + dy[k] }) &&
                        boards[i][j] == getValue({ i + 3 * dx[k], j + 3 * dy[k] }) &&
                        getValue({ i + 2 * dx[k], j + 2 * dy[k] }) == SPACE &&
                        boards[i][j] == getValue({ i + 4 * dx[k], j + 4 * dy[k] })) {
                        a[i + 2 * dx[k]][j + 2 * dy[k]] = true;
                        return true;
                    }
                    // 1 0 1 1 1
                    else if (getValue({ i, j }) == getValue({ i + 2 * dx[k], j + 2 * dy[k] }) &&
                        getValue({ i, j }) == getValue({ i + 3 * dx[k], j + 3 * dy[k] }) &&
                        getValue({ i, j }) == getValue({ i + 4 * dx[k], j + 4 * dy[k] }) &&
                        getValue({ i + dx[k], j + dy[k] }) == SPACE) {
                        a[i + 1 * dx[k]][j + 1 * dy[k]] = true;
                        return true;
                    }
                }
        }
    }
    return false;
}

void board::initZobrist() {
    std::default_random_engine e(std::clock());
    std::uniform_int_distribution<int> u(INT32_MIN, INT32_MAX);
    for(auto& i : Zobrist)
        for(auto& j : i)
            for(auto& k : j)
                k = u(e);
}
