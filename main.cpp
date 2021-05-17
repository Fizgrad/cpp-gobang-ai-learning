//
// Created by Fiz on 4/30/2021.
//
#include <thread>
#include <iostream>
#include <random>
#include "board.h"
#include "gobangAI.h"

board game;
gobangAI ai;

void modeOfAiVsAi() {
    int difficulty;
    std::cout<<"select AI level"<<std::endl;
    do{
        std::cout<<"1.Stupid\n2.Very Stupid"<<std::endl;
        std::cin>>difficulty;
    }while(difficulty!=1&&difficulty!=2);
    while (!game.isEnd()) {
        ai.findTheBest(game, difficulty==1?4:2, game.getTurns());
        game.place(ai.best);
        //std::cout << "Press a key to continue" << std::endl;
       // std::cin.get();
        system("cls");
        game.display();
        game.testEnd();
        game.logs();
    }
    std::cout<<"Over, press q to continue"<<std::endl;
    while (std::cin.get() != 'q');
}

void modeOfAiVsPeople() {
    using namespace std;
    int roleOfPlayer;
    do {
        cout << "please select your chess, 1 represents white, -1 represent black" << endl;
        cin >> roleOfPlayer;
    }while(roleOfPlayer!=1&&roleOfPlayer!=-1);
    int difficulty;
    std::cout<<"select AI level"<<std::endl;
    do{
        std::cout<<"1.Stupid\n2.Very Stupid"<<std::endl;
        std::cin>>difficulty;
    }while(difficulty!=1&&difficulty!=2);

    if (roleOfPlayer == BLACK) {
        cout << "Please input a coordinate, using decimal" << endl;
        int x, y;
        cin >> x >> y;
        game.place({x, y});
    }
    while (!game.isEnd()) {
        ai.findTheBest(game, difficulty==1?4:2, -roleOfPlayer);
        game.place(ai.best);
        game.testEnd();
        system("cls");
        game.display();
        cout<<"AI places "<<ai.best.x<<","<<ai.best.y<<endl;
        if(!game.isEnd()) {
           
            do {
                cout << "Please input a coordinate, using decimal" << endl;
                int x, y;
                cin >> x >> y;
                game.place({x, y});
                game.testEnd();
            } while (game.getTurns() == roleOfPlayer);
        }
        else {
            if(game.getWinner()==roleOfPlayer)
            {
                cout<<"You win, press q to continue"<<endl;
              
            }
            else {
                cout<<"You lose, press q to continue"<<endl;
              
            }
            while (cin.get() != 'q');
        }
    }
}

int exec() {
    using namespace std;
    board::initZobrist();
    game.initBoards();
    cout<<"Detected "<<thread::hardware_concurrency()<<" cpus"<<endl;
    cout << "select mode: \n1. AI Vs AI\n2. People Vs AI" << endl;
    int mode;
    cin >> mode;
    switch (mode) {
        case 1:
            modeOfAiVsAi();
            break;
        case 2:
            modeOfAiVsPeople();
            break;
        default:
            exec();
            break;
    }
    return 0;
}

int main() {
    return exec();
}

