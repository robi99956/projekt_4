#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>

#include "myview.h"
#include "ramie.h"
#include "fizyka.h"

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
    void rysuj( QPoint p0, QPoint p1, QPoint p2 );

private:
    Ui::MainWindow *ui;
    myGraphicsView * scena;

    QGraphicsScene *s;

    QRectF rect;

    QVector<obiekt> klocki;
    QGraphicsItem * trzymany;

    ramie * robot;
    fizyka * spadanie;
};

#endif // MAINWINDOW_H
