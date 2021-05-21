#include "dialogofabout.h"
#include "ui_dialogofabout.h"

DialogOfAbout::DialogOfAbout(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogOfAbout)
{
    ui->setupUi(this);
}

DialogOfAbout::~DialogOfAbout()
{
    delete ui;
}
