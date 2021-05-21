#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "dialogofabout.h"
#include <QGraphicsView>
#include <QGraphicsEllipseItem>
#include <QGraphicsLineItem>
#include <QGraphicsRectItem>
#include <QMouseEvent>
#include <QDebug>
#include <string>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ai=nullptr;
    game=nullptr;
    ui->setupUi(this);
    this->scene = new QGraphicsScene();
    iniScene();
    ui->graphicsView->setScene(this->scene);
    ui->AIStartCompute->setDisabled(true);
    restart();
}

MainWindow::~MainWindow()
{
    delete ui;
    delete ai;
    delete game;
}


void MainWindow::on_actAbout_triggered()
{
    QDialog* a = new DialogOfAbout();
    a->show();
}


void MainWindow::on_actSetAIVSAI_triggered()
{
    ui->aiPlace->setText("");
    QString a = "当前电脑智商：";
    if(aiLevel==4)a += ("入门");
    else {
        a += "傻子";
    }
    ui->actionSetAILevel2->setDisabled(false);
    ui->actionSetAILevel4->setDisabled(false);
    ui->AILevel->setText(a);

    this->gameMode = AIVSAI;
    this->ui->GameMode->setText("当前模式：电脑对战");
    this->ui->AIStartCompute->setDisabled(false);
    restart();
    changeGameState();
    changeWhoText();

}


void MainWindow::on_actionSetPeopleVSAI_triggered()
{
    ui->aiPlace->setText("");
    this->gameMode = AIVSPEOPLE;
    ui->actionSetAILevel2->setDisabled(false);
    ui->actionSetAILevel4->setDisabled(false);
    ui->actionSetPlayerBlack->setDisabled(false );
    ui->actionSetPlayerWhite->setDisabled(false );
    this->ui->GameMode->setText("当前模式：玩家对战电脑");
    this->ui->AIStartCompute->setDisabled(true);
    restart();
    changeGameState();
    QString a = "当前电脑智商：";
    if(aiLevel==4)a += ("入门");
    else {
        a += "傻子";
    }
    ui->AILevel->setText(a);
    changeWhoText();
}


void MainWindow::on_actSetPeopleVSPeople_triggered()
{
    ui->aiPlace->setText("");
    this->gameMode = PVSP;
    this->ui->GameMode->setText("当前模式：玩家对战");
    ui->AIStartCompute->setDisabled(true);
    ui->AILevel->setText("无AI");
    ui->actionSetAILevel2->setDisabled(true);
    ui->actionSetAILevel4->setDisabled(true);
    ui->actionSetPlayerBlack->setDisabled(true);
    ui->actionSetPlayerWhite->setDisabled(true);
    restart();
    changeGameState();
    changeWhoText();
}


void MainWindow::on_actionSetAILevel4_triggered()
{
    this->aiLevel = 4;
    ui->AILevel->setText("当前电脑智商：入门");
    restart();
}


void MainWindow::on_actionReStart_triggered()
{
    restart();
    changeWhoToPlace();
    changeGameState();
}


void MainWindow::on_actionSetAILevel2_triggered()
{
    this->aiLevel = 2;
    ui->AILevel->setText("当前电脑智商：傻子");
    restart();
}

void MainWindow::restart()
{
    if(ai!=nullptr)
    delete ai;
    if(game!=nullptr)
    delete game;
    ai = new gobangAI();
    game = new board();
    gameState=0;
    board::initZobrist();
    game->initBoards();
    iniScene();
}

void MainWindow::changeWhoToPlace()
{
    this->whoToPlace = game->getTurns();
    changeWhoText();
}



void MainWindow::iniScene()
{
    this->scene->clear();
    QGraphicsRectItem* r = new QGraphicsRectItem (-12,-12,360,360);
    scene->addItem(r);
    for(int i = 0;i<SIZE;++i)
    {
        QGraphicsLineItem* a= new QGraphicsLineItem(0,i*24,336,i*24);
        this->scene->addItem(a);
    }
    for(int i = 0;i<SIZE;++i)
    {
        QGraphicsLineItem* a= new QGraphicsLineItem(i*24,0,i*24,336);
        this->scene->addItem(a);
    }
    QGraphicsEllipseItem* circle = new QGraphicsEllipseItem(15*12 -12-3,15*12-12-3,6,6);
    circle->setBrush(Qt::black);
    this->scene->addItem(circle);
    circle = new QGraphicsEllipseItem(7*12 -12-3,7*12-12-3,6,6);
    circle->setBrush(Qt::black);
    this->scene->addItem(circle);
    circle = new QGraphicsEllipseItem(7*12 -12-3,23*12-12-3,6,6);
    circle->setBrush(Qt::black);
    this->scene->addItem(circle);
    circle = new QGraphicsEllipseItem(23*12 -12-3,23*12-12-3,6,6);
    circle->setBrush(Qt::black);
    this->scene->addItem(circle);
    circle = new QGraphicsEllipseItem(23*12 -12-3,7*12-12-3,6,6);
    circle->setBrush(Qt::black);
    this->scene->addItem(circle);
}

void MainWindow::AIComputeOnce()
{
    changeWhoToPlace();
    if(!game->isEnd()){
    ai->findTheBest(*game,this->aiLevel,this->whoToPlace);
    game->place(ai->best);
    updateScene(ai->best,whoToPlace);
    //game->display();
    }
    gameState=game->getWinner();
    QString a("AI下在了(");
    if(ai->best.x>10)
    {
        a.append('1');
        a.append(QChar(ai->best.x-10+'0'));
    }else {
        a.append(QChar(ai->best.x+'0'));
    }
    a.append(",");
    if(ai->best.y>10)
    {
        a.append('1');
        a.append(QChar(ai->best.y-10+'0'));
    }else a.append(QChar(ai->best.y+'0'));
    a.append(")");
    ui->aiPlace->setText(a);
    changeGameState();
    changeWhoToPlace();
}

void MainWindow::updateScene(const coordinate &i,int role)
{
    QGraphicsEllipseItem* a = new QGraphicsEllipseItem(i.y*24-12,i.x*24-12,24,24);
    if(role == BLACK)
        a->setBrush(Qt::black);
    else a->setBrush(Qt::white);
    this->scene->addItem(a);
}

void MainWindow::changeGameState()
{
    if(gameState==0)
    {
        ui->GameState->setText("当前状态：棋局正在进行");
    }
    else if(gameState==1)
    {
        ui->GameState->setText("当前状态：白棋获胜");
        ui->AIStartCompute->setDisabled(true);
    }
    else if(gameState==-1)
    {
        ui->GameState->setText("当前状态：黑棋获胜");
        ui->AIStartCompute->setDisabled(true);
    }
    else
    {
        ui->GameState->setText("当前状态：状态异常，请联系开发者");
    }
}


void MainWindow::on_AIStartCompute_clicked()
{
    if(gameMode==AIVSAI||(gameMode==0&&game->getTurns()!=PlayerChess))
    this->AIComputeOnce();
}


void MainWindow::on_actionSetPlayerWhite_triggered()
{
    PlayerChess = WHITE;
    ui->actionSetPlayerBlack->setDisabled(false);
    ui->actionSetPlayerWhite->setDisabled(true);
    if(gameMode==AIVSPEOPLE)
    {
        if(game->getTurns()!=PlayerChess)
           AIComputeOnce();
    }
}


void MainWindow::on_actionSetPlayerBlack_triggered()
{
    PlayerChess = BLACK;
    ui->actionSetPlayerWhite->setDisabled(false);
    ui->actionSetPlayerBlack->setDisabled(true);
    if(gameMode==AIVSPEOPLE){
        if(game->getTurns()!=PlayerChess)
        AIComputeOnce();
    }
}
void MainWindow::changeWhoText()
{
    if(!game->isEnd())
    ui->WhoToPlace->setText(whoToPlace==BLACK?"等待黑棋落子":"等待白棋落子");
    else {
        ui->WhoToPlace->setText("棋局已经结束");
    }
}

void MainWindow::playerPlace(coordinate p)
{
    if(!game->isEnd()){
        if(gameMode==PVSP)
        {
            updateScene(p,game->getTurns());
            game->place(p);
            changeWhoToPlace();
            changeWhoText();
            gameState = game->isEnd();
            changeGameState();
            ui->AIStartCompute->setDisabled(true);
        }
        if(gameMode==AIVSPEOPLE&&whoToPlace==PlayerChess)
        {
            updateScene(p,PlayerChess);
            game->place(p);
            gameState=game->isEnd();
            changeGameState();
            changeWhoToPlace();
            changeWhoText();
            this->AIComputeOnce();
            ui->AIStartCompute->setDisabled(true);
        }
     }
}

void MainWindow::mousePressEvent(QMouseEvent *event)
{
    if(event->button()==Qt::LeftButton)
    {

        QPointF ptConverted = ui->graphicsView->mapToScene(event->pos());
        std::cerr<<ptConverted.x()<<"  "<<ptConverted.y()<<std::endl;
        float x = ptConverted.y();
        float y = ptConverted.x();
        coordinate p( (x-12)/24, (y)/24);
        std::cerr<<p.x<<"  "<<p.y<<std::endl;
        if(game->getValue(p)==SPACE)
            playerPlace(p);
    }
}

