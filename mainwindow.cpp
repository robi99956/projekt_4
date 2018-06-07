#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QGraphicsItem>

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

    robot = new ramie(200, 200, QPoint( scena->width()/2, scena->height()/2) );

    connect(scena, SIGNAL(mysza(QPoint)), robot, SLOT(ustaw(QPoint)));
    connect(robot, SIGNAL(rysuj(QPoint,QPoint,QPoint)), this, SLOT(rysuj(QPoint,QPoint,QPoint)));

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

void MainWindow::rysuj(QPoint p0, QPoint p1, QPoint p2)
{
    for( int i=0; i<klocki.size(); i++ )
    {
        s->removeItem( klocki[i] );
    }

    s->clear();
    s->setSceneRect(rect);

    for( int i=0; i<klocki.size(); i++ )
    {
        s->addItem( klocki[i] );
    }

    s->addLine( p0.x(), p0.y(), p1.x(), p1.y() );
    s->addLine( p1.x(), p1.y(), p2.x(), p2.y() );
}
