//
// Created by Fiz on 4/30/2021.
//

#include "coordinate.h"
#include "board.h"
#include <iostream>
#include <fstream>

int board::getTurns() const {return this->turns%2?WHITE:BLACK;}

board::board() {
    initBoards();
    initCheckedList();
    turns = 0;
    winner = SPACE;
}

board::board(const board & b) {
    this->turns=b.turns;
    this->winner = b.winner;
    for(int i = 0 ;i<SIZE;++i)
        for(int j = 0 ;j<SIZE;++j)
            this->boards[i][j]=b.boards[i][j];
}

void board::initBoards() {
    for (auto &i : boards)
        for (auto &j : i)
            j = SPACE;
}

inline void board::initCheckedList() {
    for (auto &i : checked)
        for (auto &j : i)
            for (auto &k :j)
                k = false;
}

inline bool board::setCheckedTrue(const coordinate &p, int direction) {
    if (p.x < SIZE && p.x >= 0 && p.y >= 0 && p.y < SIZE)
        return checked[p.x][p.y][direction] = true;
    else return false;
}

bool board::isEnd() const {
    return this->winner != SPACE || this->turns >= SIZE * SIZE;
}

//place a chess at given coordinate. the type depends on turns.

board& board::place(const coordinate &p) {
    if (winner == SPACE && p.x < SIZE && p.x >= 0 && p.y >= 0 && p.y < SIZE && boards[p.x][p.y] == SPACE) {
        this->boards[p.x][p.y] = (turns++ % 2 ? WHITE : BLACK);
    }
    return *this;
}
board& board::unplace(const coordinate &p) {
    if (winner == SPACE && p.x < SIZE && p.x >= 0 && p.y >= 0 && p.y < SIZE && boards[p.x][p.y] != SPACE) {
        this->boards[p.x][p.y] = SPACE;
        --turns;
    }
    return *this;
}

//to return the value of the chess placed at the given and valid coordinate,if the given coordinate is invalid,
//return the value of EDGE, AKA 2, which doesn't equal to white or black.
int board::getValue(const coordinate &p) const{
    return (p.x >= SIZE || p.y >= SIZE || p.x < 0 || p.y < 0) ? EDGE : this->boards[p.x][p.y];
}

//to check whether someone is winner
int board::testEnd() {
    initCheckedList();
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
                        std::cout<<"Test end"<<std::endl;
                        this->winner = boards[i][j];
                        return this->winner;

                    }else {
                        for (int temp = 0; temp <= 4; ++temp)
                        {
                            if(boards[i][j]==getValue({i+temp*dx[k],j+temp*dy[k]}))
                                setCheckedTrue({i+temp*dx[k],j+temp*dy[k]},k);
                            else break;
                        }
                    }
                }
        }
    }
    return SPACE;
}
//print the board
void board::display() {
    using std::cout;
    cout<<turns<<": ";
    cout<<(turns%2?"B":"W")<<std::endl;
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
    cout << "Evaluate: B " <<evaluate(BLACK)<<" W "<<evaluate(WHITE)<< std::endl<<std::endl;
    if (winner != SPACE)
        cout << "winner is " << winner << std::endl;
}
//print the information of each step in a file
void board::logs(const std::string &filename) {
    std::ofstream out(filename, std::ios::app);
    out<<turns<<": ";
    out<<(turns%2?"B":"W")<<std::endl;
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
    out << "Evaluate: B " <<evaluate(BLACK)<<" W "<<evaluate(WHITE)<< std::endl<<std::endl;
    if (winner != SPACE)
        out << "winner is " << winner << std::endl;
}

//analyze the whole situation
int board::evaluateOverall() {
    initCheckedList();
    int result = evaluate(BLACK)+evaluate(WHITE);
    return result;
}
//white more is better, black less is better
int board::evaluate(int role) {
    initCheckedList();
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
                            result += getValue({i, j}) * (100000000);
                            for (int temp = 0; temp <= 4; ++temp)
                                setCheckedTrue({i + temp * dx[k], j + temp * dy[k]}, k);
                        }
                            // 0 1 1 1 1 0
                        else if (boards[i][j] == getValue({i + dx[k], j + dy[k]}) &&
                                 boards[i][j] == getValue({i + 2 * dx[k], j + 2 * dy[k]}) &&
                                 boards[i][j] == getValue({i + 3 * dx[k], j + 3 * dy[k]}) &&
                                 getValue({i + 4 * dx[k], j + 4 * dy[k]}) == SPACE &&
                                 getValue({i - dx[k], j - dy[k]}) == SPACE) {
                            result += getValue({i, j}) * (1000000);
                            for (int temp = 0; temp <= 3; ++temp)
                                setCheckedTrue({i + temp * dx[k], j + dy[k] * temp}, k);
                        }
                            // 0 1 1 1 1 # or # 1 1 1 1 0
                        else if (boards[i][j] == getValue({i + dx[k], j + dy[k]}) &&
                                 boards[i][j] == getValue({i + 2 * dx[k], j + 2 * dy[k]}) &&
                                 boards[i][j] == getValue({i + 3 * dx[k], j + 3 * dy[k]}) &&
                                 (getValue({i + 4 * dx[k], j + 4 * dy[k]}) == SPACE ||
                                  getValue({i - dx[k], j - dy[k]}) == SPACE)) {
                            result += getValue({i, j}) * (100000);
                            for (int temp = 0; temp <= 3; ++temp)
                                setCheckedTrue({i + temp * dx[k], j + dy[k] * temp}, k);
                        }
                            // # 1 1 1 1 #
                        else if (boards[i][j] == getValue({i + dx[k], j + dy[k]}) &&
                                 boards[i][j] == getValue({i + 2 * dx[k], j + 2 * dy[k]}) &&
                                 boards[i][j] == getValue({i + 3 * dx[k], j + 3 * dy[k]})){
                            result += getValue({i, j}) * (10000);
                            for (int temp = 0; temp <= 3; ++temp)
                                setCheckedTrue({i + temp * dx[k], j + dy[k] * temp}, k);
                        }
                            // 1 1 1 0 1
                        else if (boards[i][j] == getValue({i + dx[k], j + dy[k]}) &&
                                 boards[i][j] == getValue({i + 2 * dx[k], j + 2 * dy[k]}) &&
                                 getValue({i + 3 * dx[k], j + 3 * dy[k]}) == SPACE &&
                                 boards[i][j] == getValue({i + 4 * dx[k], j + 4 * dy[k]})) {
                            result += getValue({i, j}) * (91000);
                            for (int temp = 0; temp <= 4; ++temp)
                                setCheckedTrue({i + temp * dx[k], j + dy[k] * temp}, k);
                        }
                            // 1 1 0 1 1
                        else if (boards[i][j] == getValue({i + dx[k], j + dy[k]}) &&
                                 boards[i][j] == getValue({i + 3 * dx[k], j + 3 * dy[k]}) &&
                                 getValue({i + 2 * dx[k], j + 2 * dy[k]}) == SPACE &&
                                 boards[i][j] == getValue({i + 4 * dx[k], j + 4 * dy[k]})) {
                            result += getValue({i, j}) * (90000);
                            for (int temp = 0; temp <= 4; ++temp)
                                setCheckedTrue({i + temp * dx[k], j + dy[k] * temp}, k);
                        }
                            // 1 0 1 1 1
                        else if (getValue({i, j}) == getValue({i + 2 * dx[k], j + 2 * dy[k]}) &&
                                 getValue({i, j}) == getValue({i + 3 * dx[k], j + 3 * dy[k]}) &&
                                 getValue({i, j}) == getValue({i + 4 * dx[k], j + 4 * dy[k]}) &&
                                 getValue({i + dx[k], j + dy[k]}) == SPACE) {
                            result += getValue({i, j}) * (90000);
                            for (int temp = 0; temp <= 4; ++temp)
                                setCheckedTrue({i + temp * dx[k], j + dy[k] * temp}, k);
                        }
                            // 0 1 0 1 1 0
                        else if (getValue({i, j}) == getValue({i + 2 * dx[k], j + 2 * dy[k]}) &&
                                 getValue({i, j}) == getValue({i + 3 * dx[k], j + 3 * dy[k]}) &&
                                 getValue({i + 4 * dx[k], j + 4 * dy[k]}) == SPACE &&
                                 getValue({i + dx[k], j + dy[k]}) == SPACE &&
                                 getValue({i - dx[k], j - dy[k]}) == SPACE) {
                            result += getValue({i, j}) * (90000);
                            for (int temp = 0; temp <= 3; ++temp)
                                setCheckedTrue({i + temp * dx[k], j + dy[k] * temp}, k);
                        }
                            // 0 1 1 0 1 0
                        else if (getValue({i, j}) == getValue({i + dx[k], j + dy[k]}) &&
                                 getValue({i, j}) == getValue({i + 3 * dx[k], j + 3 * dy[k]}) &&
                                 getValue({i + 4 * dx[k], j + 4 * dy[k]}) == SPACE &&
                                 getValue({i + 2 * dx[k], j + 2 * dy[k]}) == SPACE &&
                                 getValue({i - dx[k], j - dy[k]}) == SPACE) {
                            result += getValue({i, j}) * (90000);
                            for (int temp = 0; temp <= 3; ++temp)
                                setCheckedTrue({i + temp * dx[k], j + dy[k] * temp}, k);
                        }
                            // 0 1 1 1 0
                        else if (getValue({i, j}) == getValue({i + dx[k], j + dy[k]}) &&
                                 getValue({i, j}) == getValue({i + 2 * dx[k], j + 2 * dy[k]}) &&
                                 getValue({i + 3 * dx[k], j + 3 * dy[k]}) == SPACE &&
                                 getValue({i - dx[k], j - dy[k]}) == SPACE) {
                            result += getValue({i, j}) * 100000;
                            for (int temp = 0; temp <= 2; ++temp)
                                setCheckedTrue({i + temp * dx[k], j + dy[k] * temp}, k);
                        }
                            // 1 1 1 0 0
                        else if (getValue({i, j}) == getValue({i + dx[k], j + dy[k]}) &&
                                 getValue({i, j}) == getValue({i + 2 * dx[k], j + 2 * dy[k]}) &&
                                 getValue({i + 3 * dx[k], j + 3 * dy[k]}) == SPACE &&
                                 getValue({i + 4 * dx[k], j + 4 * dy[k]}) == SPACE) {
                            result += getValue({i, j}) * 11000;
                            for (int temp = 0; temp <= 2; ++temp)
                                setCheckedTrue({i + temp * dx[k], j + dy[k] * temp}, k);
                        }
                            // 0 0 1 1 1
                        else if (getValue({i, j}) == getValue({i + dx[k], j + dy[k]}) &&
                                 getValue({i, j}) == getValue({i + 2 * dx[k], j + 2 * dy[k]}) &&
                                 getValue({i - dx[k], j - dy[k]}) == SPACE &&
                                 getValue({i - 2 * dx[k], j - 2 * dy[k]}) == SPACE) {
                            result += getValue({i, j}) * 11000;
                            for (int temp = 0; temp <= 2; ++temp)
                                setCheckedTrue({i + temp * dx[k], j + dy[k] * temp}, k);
                        }
                            // 1 1 0 1 0
                        else if (getValue({i, j}) == getValue({i + dx[k], j + dy[k]}) &&
                                 getValue({i, j}) == getValue({i + 3 * dx[k], j + 3 * dy[k]}) &&
                                 getValue({i + 2 * dx[k], j + 2 * dy[k]}) == SPACE &&
                                 getValue({i + 4 * dx[k], j + 4 * dy[k]}) == SPACE) {
                            result += getValue({i, j}) * 9000;
                            for (int temp = 0; temp <= 3; ++temp)
                                setCheckedTrue({i + temp * dx[k], j + dy[k] * temp}, k);
                        }
                            // 0 1 1 0
                        else if (getValue({i, j}) == getValue({i + dx[k], j + dy[k]}) &&
                                 getValue({i + 2 * dx[k], j + 2 * dy[k]}) == SPACE &&
                                 getValue({i - dx[k], j - dy[k]}) == SPACE) {
                            result += getValue({i, j}) * (1000);
                            //std::cout<<turns<<": "<<i<<" "<<j<<" "<<k<<std::endl;
                            for (int temp = 0; temp <= 1; ++temp)
                                setCheckedTrue({i + temp * dx[k], j + dy[k] * temp}, k);
                        }
                        // 0 1 0
                        else if (
                                 getValue({i +   dx[k], j +  dy[k]}) == SPACE &&
                                 getValue({i - dx[k], j - dy[k]}) == SPACE) {
                            result += getValue({i, j}) * (5);
                            //std::cout<<turns<<": "<<i<<" "<<j<<" "<<k<<std::endl;
                            for (int temp = 0; temp <= 0; ++temp)
                                setCheckedTrue({i + temp * dx[k], j + dy[k] * temp}, k);
                        }
                        // 0 1 0 1 0
                        else if (getValue({i, j}) == getValue({i + 2*dx[k], j + 2*dy[k]}) &&
                                 getValue({i + dx[k], j +  dy[k]}) == SPACE &&
                                 getValue({i - dx[k], j - dy[k]}) == SPACE&&
                                getValue({i + 3 * dx[k], j + 3 * dy[k]}) == SPACE) {
                            result += getValue({i, j}) * (500);
                            //std::cout<<turns<<": "<<i<<" "<<j<<" "<<k<<std::endl;
                            for (int temp = 0; temp <= 2; ++temp)
                                setCheckedTrue({i + temp * dx[k], j + dy[k] * temp}, k);
                        }
                    }
                }
        }
    }

    return result;
}