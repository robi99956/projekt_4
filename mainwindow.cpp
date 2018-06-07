#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QGraphicsItem>

#define _USE_MATH_DEFINES
#include <math.h>

#include "kinematyka.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    scena = new myGraphicsView;
    scena->setGeometry( 25, 25, 800, 800 );
    scena->setParent(this);
    scena->show();

    scena->setHorizontalScrollBarPolicy( Qt::ScrollBarAlwaysOff );
    scena->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    s = new QGraphicsScene(0, 0, scena->width(), scena->height());
    scena->setScene(s);

    connect(scena, SIGNAL(mysza(QPoint)), this, SLOT(rysuj(QPoint)));

    rect = s->sceneRect();

    QPixmap map(":/kw.png");
    map = map.scaled(50, 50);

    QGraphicsItem * wsk;

    for( int i=0; i<4; i++ )
    {
        wsk = s->addPixmap(map);
        wsk->setPos(20+60*i, 500);

        klocki.push_back(wsk);
    }

    trzymany = NULL;
}

MainWindow::~MainWindow()
{
    delete ui;
    delete scena;
}

void MainWindow::rysuj(QPoint punkt)
{
    int w = scena->width()/2;
    int h = scena->height()/2;
    kinematyka k(200, 200, QPoint(w, h) );

    if( trzymany ) trzymany->setPos( punkt.x(), punkt.y() );

    QPoint p = k.przelicz(punkt);

    if( p.x() == 0 && p.y() == 0 ) return;

    for( int i=0; i<klocki.size(); i++ )
    {
        s->removeItem( klocki[i] );
    }

    s->clear();
    s->setSceneRect(rect);

    int x1 = p.x();
    int y1 = p.y();

    s->addLine(w, h, w+x1, h-y1);
    s->addLine(w+x1, h-y1, punkt.x(), punkt.y());

    for( int i=0; i<klocki.size(); i++ )
    {
        s->addItem( klocki[i] );
    }


    QGraphicsItem * it = s->itemAt(punkt, QTransform());

    if( it->pos().isNull() == 0 )
    {
        if( trzymany == NULL ) trzymany = it;
    }
}
