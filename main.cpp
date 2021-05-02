//
// Created by Fiz on 4/30/2021.
//

#include <iostream>

#include <random>
#include "board.h"
#include "gobangAI.h"

board game;
gobangAI ai;

void testAi()
{
    std::default_random_engine e;
    std::uniform_int_distribution<int> u(0,14);    //close
    while (!game.isEnd()) {
        if(game.getTurns()==WHITE)
            game.place({ u(e), u(e) });
        else {
            ai.findTheBest(game, 2, game.getTurns());
            game.place(ai.best);
        }
        //game.display();
        game.testEnd();
        game.logs();
        std::cout<<game.getTurns()<<": "<<game.evaluateOverall()<<std::endl;
    }
}

void testEvaluate()
{
    std::default_random_engine e;
    std::uniform_int_distribution<int> u(0,14);    //close
    game.initBoards();
    while (!game.isEnd()) {
        game.place({ u(e), u(e) });
        game.testEnd();
        game.logs();
        std::cout<<game.getTurns()<<": "<<game.evaluateOverall()<<std::endl;
    }
}

int main() {
    std::cout << "Hello, World!" << std::endl;

    game.initBoards();
//    testEvaluate();
    testAi();
    return 0;
}

