#ifndef DIALOGOFABOUT_H
#define DIALOGOFABOUT_H

#include <QDialog>

namespace Ui {
class DialogOfAbout;
}

class DialogOfAbout : public QDialog
{
    Q_OBJECT

public:
    explicit DialogOfAbout(QWidget *parent = nullptr);
    ~DialogOfAbout();

private:
    Ui::DialogOfAbout *ui;
};

#endif // DIALOGOFABOUT_H
