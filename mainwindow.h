#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "gobangai.h"
#include "board.h"
#include <QGraphicsScene>
#include <QGraphicsItem>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

const int AIVSAI = 1;
const int AIVSPEOPLE = 0;
const int PVSP = -1;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    int gameMode = 4;
    int whoToPlace = BLACK;
    int aiLevel = 4;
    int gameState;
    int PlayerChess = BLACK;
    gobangAI* ai;
    board* game;
    QGraphicsScene* scene;

private slots:
    void on_actAbout_triggered();

    void on_actSetAIVSAI_triggered();

    void on_actionSetPeopleVSAI_triggered();

    void on_actSetPeopleVSPeople_triggered();

    void on_actionSetAILevel4_triggered();

    void on_actionReStart_triggered();

    void on_actionSetAILevel2_triggered();

    void restart();

    void changeWhoToPlace();

    void iniScene();

    void AIComputeOnce();

    void updateScene(const coordinate& i,int role);

    void changeGameState();

    void on_AIStartCompute_clicked();

    void on_actionSetPlayerWhite_triggered();

    void on_actionSetPlayerBlack_triggered();

    void changeWhoText();

    void playerPlace(coordinate p);

    void mousePressEvent(QMouseEvent* event);
private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
