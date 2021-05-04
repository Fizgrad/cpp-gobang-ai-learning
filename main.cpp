//
// Created by Fiz on 4/30/2021.
//

#include <iostream>

#include <random>
#include "board.h"
#include "gobangAI.h"

board game;
gobangAI ai;
int board::Zobrist[SIZE][SIZE][3] = {};

void aiVSai() {
    while (!game.isEnd()) {
        ai.findTheBest(game, 4, game.getTurns());
        game.place(ai.best);
        system("cls");
        game.display();
        game.testEnd();
        game.logs();
    }

}


void aiVSpeople() {
    using namespace std;
    cout<<"please select your chess, 1 represents white, -1 represent black"<<endl;
    int roleOfPlayer;
    cin>>roleOfPlayer;
    if(roleOfPlayer == BLACK)
    {
        cout<<"Please input a coordinate, using decimal"<<endl;
        int x,y;
        cin>>x>>y;
        game.place({x,y});
    }
    while(!game.isEnd())
    {
        ai.findTheBest(game,4,-roleOfPlayer);
        game.place(ai.best);
        game.display();
        do {
            cout << "Please input a coordinate, using decimal" << endl;
            int x, y;
            cin >> x >> y;
            game.place({x, y});
        }while(game.getTurns()==roleOfPlayer);
    }
}

void testEvaluate() {
    std::default_random_engine e;
    std::uniform_int_distribution<int> u(0, 14);    //close
    game.initBoards();
    while (!game.isEnd()) {
        game.place({u(e), u(e)});
        game.testEnd();
        game.logs();
    }
}

int main() {
    std::cout << "Hello, World!" << std::endl;
    board::initZobrist();
    game.initBoards();
//    testEvaluate();
    aiVSai();
//    aiVSpeople();
    return 0;
}

