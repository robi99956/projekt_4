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
    void narysuj_ramie(QPoint p0, QPoint p1);
    ~MainWindow();

private slots:
    void rysuj( QPoint p0, QPoint p1, QPoint p2 );

    void on_horizontalSlider_sliderMoved(int position);
    void nagraj(int stan);

    void on_rec_play_clicked();

private:
    Ui::MainWindow *ui;
    myGraphicsView * scena;

    QGraphicsScene *s;

    QRectF rect;

    QVector<obiekt> klocki;
    QVector<obiekt> meble;
    QGraphicsItem * trzymany;

    QPixmap bck;
    QPixmap nju;

    ramie * robot;
    fizyka * spadanie;

    int total_records = 0;
    QPalette kol_red;
    QPalette kol_green;
    QPalette kol_blue;

};

#endif // MAINWINDOW_H
