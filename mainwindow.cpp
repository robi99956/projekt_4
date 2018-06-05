#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>

#define _USE_MATH_DEFINES
#include "math.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    scena = new myGraphicsView;
    scena->setGeometry( 25, 25, 500, 500 );
    scena->setParent(this);
    scena->show();

    scena->setHorizontalScrollBarPolicy( Qt::ScrollBarAlwaysOff );
    scena->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    s = new QGraphicsScene(0, 0, scena->width(), scena->height());
    scena->setScene(s);

    connect(scena, SIGNAL(mysza(QPoint)), this, SLOT(rysuj(QPoint)));

    rect = s->sceneRect();
}

MainWindow::~MainWindow()
{
    delete ui;
    delete scena;
}

void MainWindow::rysuj(QPoint punkt)
{
    s->clear();
    s->setSceneRect( rect );

    int w = scena->width()/2;
    int h = scena->height()/2;
    punkt.setX( punkt.x() - w );
    punkt.setY( h - punkt.y() );

    int l0 = 200;
    int l1 = 200;
    int x = punkt.x();
    int y = punkt.y();

    double d = sqrt( x*x+y*y);

    double alfa = atan2(y, x);
    double beta = acos( (l1*l1-l0*l0+d*d)/(2*d*l1) );
    double fi = M_PI*3/2 - beta-alfa;

    int x1 = x+ l1*cos(fi);
    int y1 = y+ l1*sin(fi);

    s->addLine(w, h, w+x1, h-y1);
    s->addLine(w+x1, h-y1, w+x, h-y);
}
