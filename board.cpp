//
// Created by Fiz on 4/30/2021.
//

#include "coordinate.h"
#include "board.h"
#include <iostream>
#include <fstream>

int board::getTurns() const {return this->turns;}

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
}

bool board::isEnd() const {
    return this->winner != SPACE || this->turns >= SIZE * SIZE;
}

bool board::place(const coordinate &p) {
    if (winner == SPACE && p.x < SIZE && p.x >= 0 && p.y >= 0 && p.y < SIZE && boards[p.x][p.y] == SPACE) {
        boards[p.x][p.y] = (turns++ % 2 ? WHITE : BLACK);
        return true;
    } else return false;
}

int board::getValue(const coordinate &p) {
    return (p.x >= SIZE || p.y >= SIZE || p.x < 0 || p.y < 0) ? EDGE : this->boards[p.x][p.y];
}


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
                            boards[i][j] ==getValue({i + dx[k], j + dy[k]})?setCheckedTrue({i + temp * dx[k], j + temp * dy[k]}, k):0;
                    }
                }
        }
    }
    return SPACE;
}

void board::display() {
    for (auto &i : boards) {
        for (auto &j :i)
            switch (j) {
                case BLACK:
                    std::cout << '@';
                    break;
                case WHITE:
                    std::cout << 'O';
                    break;
                case SPACE:
                    std::cout << ' ';
                    break;
                default:
                    break;
            }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

void board::logs(const std::string &filename) {
    std::ofstream out(filename, std::ios::app);
    out<<turns<<":\n";
    out << "  ";
    for (int i = 0; i < SIZE; ++i)
        out << char(i < 10 ? i + '0' : 'A' + i - 10);
    out << std::endl << std::endl;
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

//evaluate unfinished

int board::evaluateOverall() {
    initCheckedList();
    int result = evaluate(BLACK)+evaluate(WHITE);

    return result;
}
//negative
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
                            result += getValue({i, j}) * 99999999;
                            for (int temp = 0; temp <= 4; ++temp)
                                setCheckedTrue({i + temp * dx[k], j + temp * dy[k]}, k);
                        }
                            // 0 1 1 1 1 0
                        else if (boards[i][j] == getValue({i + dx[k], j + dy[k]}) &&
                                 boards[i][j] == getValue({i + 2 * dx[k], j + 2 * dy[k]}) &&
                                 boards[i][j] == getValue({i + 3 * dx[k], j + 3 * dy[k]}) &&
                                 getValue({i + 4 * dx[k], j + 4 * dy[k]}) == SPACE &&
                                 getValue({i - dx[k], j - dy[k]}) == SPACE) {
                            result += getValue({i, j}) * 50000;
                            for (int temp = 0; temp <= 3; ++temp)
                                setCheckedTrue({i + temp * dx[k], j + dy[k] * temp}, k);
                        }
                            // 0 1 1 1 1 # or # 1 1 1 1 0
                        else if (boards[i][j] == getValue({i + dx[k], j + dy[k]}) &&
                                 boards[i][j] == getValue({i + 2 * dx[k], j + 2 * dy[k]}) &&
                                 boards[i][j] == getValue({i + 3 * dx[k], j + 3 * dy[k]}) &&
                                 (getValue({i + 4 * dx[k], j + 4 * dy[k]}) == SPACE ||
                                  getValue({i - dx[k], j - dy[k]}) == SPACE)) {
                            result += getValue({i, j}) * 5000;
                            for (int temp = 0; temp <= 3; ++temp)
                                setCheckedTrue({i + temp * dx[k], j + dy[k] * temp}, k);
                        }
                            // # 1 1 1 1 #
                        else if (boards[i][j] == getValue({i + dx[k], j + dy[k]}) &&
                                 boards[i][j] == getValue({i + 2 * dx[k], j + 2 * dy[k]}) &&
                                 getValue({i + 3 * dx[k], j + 3 * dy[k]}) != SPACE &&
                                 getValue({i - dx[k], j - dy[k]}) != SPACE) {
                            result += getValue({i, j}) * 1000;
                            for (int temp = 0; temp <= 3; ++temp)
                                setCheckedTrue({i + temp * dx[k], j + dy[k] * temp}, k);
                        }
                            // 1 1 1 0 1
                        else if (boards[i][j] == getValue({i + dx[k], j + dy[k]}) &&
                                 boards[i][j] == getValue({i + 2 * dx[k], j + 2 * dy[k]}) &&
                                 getValue({i + 3 * dx[k], j + 3 * dy[k]}) == SPACE &&
                                 boards[i][j] == getValue({i + 4 * dx[k], j + 4 * dy[k]})) {
                            result += getValue({i, j}) * 1000;
                            for (int temp = 0; temp <= 4; ++temp)
                                setCheckedTrue({i + temp * dx[k], j + dy[k] * temp}, k);
                        }
                            // 1 1 0 1 1
                        else if (boards[i][j] == getValue({i + dx[k], j + dy[k]}) &&
                                 boards[i][j] == getValue({i + 3 * dx[k], j + 3 * dy[k]}) &&
                                 getValue({i + 2 * dx[k], j + 2 * dy[k]}) == SPACE &&
                                 boards[i][j] == getValue({i + 4 * dx[k], j + 4 * dy[k]})) {
                            result += getValue({i, j}) * 5000;
                            for (int temp = 0; temp <= 4; ++temp)
                                setCheckedTrue({i + temp * dx[k], j + dy[k] * temp}, k);
                        }
                            // 1 0 1 1 1
                        else if (getValue({i, j}) == getValue({i + 2 * dx[k], j + 2 * dy[k]}) &&
                                 getValue({i, j}) == getValue({i + 3 * dx[k], j + 3 * dy[k]}) &&
                                 getValue({i, j}) == getValue({i + 4 * dx[k], j + 4 * dy[k]}) &&
                                 getValue({i + dx[k], j + dy[k]}) == SPACE) {
                            result += getValue({i, j}) * 5000;
                            for (int temp = 0; temp <= 4; ++temp)
                                setCheckedTrue({i + temp * dx[k], j + dy[k] * temp}, k);
                        }
                            // 0 1 0 1 1 0
                        else if (getValue({i, j}) == getValue({i + 2 * dx[k], j + 2 * dy[k]}) &&
                                 getValue({i, j}) == getValue({i + 3 * dx[k], j + 3 * dy[k]}) &&
                                 getValue({i + 4 * dx[k], j + 4 * dy[k]}) == SPACE &&
                                 getValue({i + dx[k], j + dy[k]}) == SPACE &&
                                 getValue({i - dx[k], j - dy[k]}) == SPACE) {
                            result += getValue({i, j}) * 5000;
                            for (int temp = 0; temp <= 3; ++temp)
                                setCheckedTrue({i + temp * dx[k], j + dy[k] * temp}, k);
                        }
                            // 0 1 1 0 1 0
                        else if (getValue({i, j}) == getValue({i + dx[k], j + dy[k]}) &&
                                 getValue({i, j}) == getValue({i + 3 * dx[k], j + 3 * dy[k]}) &&
                                 getValue({i + 4 * dx[k], j + 4 * dy[k]}) == SPACE &&
                                 getValue({i + 2 * dx[k], j + 2 * dy[k]}) == SPACE &&
                                 getValue({i - dx[k], j - dy[k]}) == SPACE) {
                            result += getValue({i, j}) * 5000;
                            for (int temp = 0; temp <= 3; ++temp)
                                setCheckedTrue({i + temp * dx[k], j + dy[k] * temp}, k);
                        }
                            // 0 1 1 1 0
                        else if (getValue({i, j}) == getValue({i + dx[k], j + dy[k]}) &&
                                 getValue({i, j}) == getValue({i + 2 * dx[k], j + 2 * dy[k]}) &&
                                 getValue({i + 3 * dx[k], j + 3 * dy[k]}) == SPACE &&
                                 getValue({i - dx[k], j - dy[k]}) == SPACE) {
                            result += getValue({i, j}) * 5000;
                            for (int temp = 0; temp <= 2; ++temp)
                                setCheckedTrue({i + temp * dx[k], j + dy[k] * temp}, k);
                        }
                            // 1 1 1 0 0
                        else if (getValue({i, j}) == getValue({i + dx[k], j + dy[k]}) &&
                                 getValue({i, j}) == getValue({i + 2 * dx[k], j + 2 * dy[k]}) &&
                                 getValue({i + 3 * dx[k], j + 3 * dy[k]}) == SPACE &&
                                 getValue({i + 4 * dx[k], j + 4 * dy[k]}) == SPACE) {
                            result += getValue({i, j}) * 500;
                            for (int temp = 0; temp <= 2; ++temp)
                                setCheckedTrue({i + temp * dx[k], j + dy[k] * temp}, k);
                        }
                            // 0 0 1 1 1
                        else if (getValue({i, j}) == getValue({i + dx[k], j + dy[k]}) &&
                                 getValue({i, j}) == getValue({i + 2 * dx[k], j + 2 * dy[k]}) &&
                                 getValue({i - dx[k], j - dy[k]}) == SPACE &&
                                 getValue({i - 2 * dx[k], j - 2 * dy[k]}) == SPACE) {
                            result += getValue({i, j}) * 500;
                            for (int temp = 0; temp <= 2; ++temp)
                                setCheckedTrue({i + temp * dx[k], j + dy[k] * temp}, k);
                        }
                            // 1 1 0 1 0
                        else if (getValue({i, j}) == getValue({i + dx[k], j + dy[k]}) &&
                                 getValue({i, j}) == getValue({i + 3 * dx[k], j + 3 * dy[k]}) &&
                                 getValue({i + 2 * dx[k], j + 2 * dy[k]}) == SPACE &&
                                 getValue({i + 4 * dx[k], j + 4 * dy[k]}) == SPACE) {
                            result += getValue({i, j}) * 200;
                            for (int temp = 0; temp <= 3; ++temp)
                                setCheckedTrue({i + temp * dx[k], j + dy[k] * temp}, k);
                        }
                            // 0 1 1 0
                        else if (getValue({i, j}) == getValue({i + dx[k], j + dy[k]}) &&
                                 getValue({i + 2 * dx[k], j + 2 * dy[k]}) == SPACE &&
                                 getValue({i - dx[k], j - dy[k]}) == SPACE) {
                            result += getValue({i, j}) * 50;
                            for (int temp = 0; temp <= 1; ++temp)
                                setCheckedTrue({i + temp * dx[k], j + dy[k] * temp}, k);
                        }
                    }
                }
        }
    }

    return result;
}