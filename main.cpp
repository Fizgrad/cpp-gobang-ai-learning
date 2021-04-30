//
// Created by Fiz on 4/30/2021.
//

#include <iostream>
#include <random>
#include "board.h"


int main() {
    std::cout << "Hello, World!" << std::endl;
    board game;
    std::default_random_engine e;
    std::uniform_int_distribution<int> u(0,14);    //close
    game.initBoards();
    while (!game.isEnd()) {
        game.place({ u(e), u(e) });
        game.testEnd();
        game.logs();
        std::cout<<game.getTurns()<<": "<<game.evaluateOverall()<<std::endl;
    }
    return 0;
}
