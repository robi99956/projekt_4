#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>

#include "myview.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void rysuj( QPoint punkt );

private:
    Ui::MainWindow *ui;
    myGraphicsView * scena;

    QGraphicsScene *s;

    QRectF rect;
};

#endif // MAINWINDOW_H
