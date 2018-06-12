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
    spadanie = new fizyka(0, scena->height() );

    connect(scena, SIGNAL(mysza(QPoint)), robot, SLOT(ustaw(QPoint)));
    connect(robot, SIGNAL(rysuj(QPoint,QPoint,QPoint)), this, SLOT(rysuj(QPoint,QPoint,QPoint)));
    connect(scena, SIGNAL(klawisz(int)), robot, SLOT(KeyEvent(int)));
    connect(robot, SIGNAL(zlapal(QGraphicsItem*)), spadanie, SLOT(zlapane(QGraphicsItem*)));

    connect(robot, SIGNAL(koniec_odtwarzania()), this, SLOT(koniec_odtwarzania()));

    //----nagrywanie pod r
    connect(robot,SIGNAL(nagrywanie(int)),this,SLOT(nagraj(int)));

    rect = s->sceneRect();

    QPixmap map(":/kw.png");
    QPixmap polka(":/polka.png");
    QPixmap szafka(":/szafka_mala.png");
    map = map.scaled(50, 50);


    //--------widgety z ui-------------
    ui->horizontalSlider->setMaximum(30);
    ui->horizontalSlider->setMinimum(4);


  //  bck.load(":/tlo.png");
  //  nju = bck.scaled(s->width(),s->height());


    QGraphicsItem * wsk;
    obiekt obj = {NULL, 0, 1};

    for( int i=0; i<4; i++ )
    {
        wsk = s->addPixmap(map);
        wsk->setPos(100+60*i, 500);

        obj.wsk = wsk;
        klocki.push_back(obj);
    }

    obiekt ikea = {NULL, 0 , 0 };
    QPixmap repolka = polka.scaled(350,polka.height()/2);
    wsk = s->addPixmap(repolka);
    wsk->setPos(450,400);
    ikea.wsk = wsk;
    klocki.push_back(ikea);

    QPixmap reszafka = szafka.scaled(270,250);
    wsk = s->addPixmap(reszafka);
    wsk->setPos(25,550);
    ikea.wsk = wsk;
    klocki.push_back(ikea);


    trzymany = NULL;
    spadanie->zarejestruj_obiekty( &klocki );


    //------qlabel colors
    ui->rec_info->setAutoFillBackground(true);
    kol_green = ui->rec_info->palette();
    kol_red = ui->rec_info->palette();
    kol_blue = ui->rec_info->palette();
    kol_green.setColor(QPalette::Window, QColor(Qt::green));
    kol_red.setColor(QPalette::Window, QColor(Qt::red));
    kol_blue.setColor(QPalette::Window, QColor::fromRgb(51,255,255));


}

MainWindow::~MainWindow()
{
    delete ui;
    delete scena;
    delete spadanie;
}

void MainWindow::rysuj(QPoint p0, QPoint p1, QPoint p2)
{
    for( int i=0; i<klocki.size(); i++ )
    {
        s->removeItem( klocki[i].wsk );
    }

    s->clear();
    s->setSceneRect(rect);
  //  s->addPixmap(nju);

    for( int i=0; i<klocki.size(); i++ )
    {
        s->addItem( klocki[i].wsk );
    }

    QGraphicsItem *it = s->itemAt(p2, QTransform());
    robot->zlap(it);


    narysuj_ramie(p0,p1);
    narysuj_ramie(p1,p2);

    s->addLine( p0.x(), p0.y(), p1.x(), p1.y() );

    s->addLine( p1.x(), p1.y(), p2.x(), p2.y() );
}


void MainWindow::narysuj_ramie(QPoint p0, QPoint p1)
{

    int dl_modul = sqrt( (p1.y()-p0.y())*(p1.y()-p0.y()) + (p1.x() - p0.x()) * (p1.x() - p0.x()));
    QPixmap modul(":/modul.png");

    QSize wymiar ( dl_modul+40, modul.height());

    QPixmap modul2 =  modul.scaled(wymiar,Qt::IgnoreAspectRatio, Qt::FastTransformation);
    QGraphicsPixmapItem *reka = new QGraphicsPixmapItem (modul2);

    QPoint movedp0;
    movedp0 = p0;

    int dx = modul.width()/6;
    int dy = modul.height()/2;
    movedp0.setY(p0.y()-dy);
    movedp0.setX(p0.x()-dx);

    reka->setPos(movedp0);
    reka->setTransformOriginPoint(dx,dy);
    double angle;
    angle = atan2(p1.y()-p0.y(),p1.x()-p0.x());
    angle = qRadiansToDegrees(angle);
    reka->setRotation(angle);
    s->addItem(reka);
}

void MainWindow::on_horizontalSlider_sliderMoved(int position)
{
    ui->speed_label->setText(QString::number(position));
}

void MainWindow::nagraj(int stan)
{
    // 1 nagrywa, -1 nie nagrywa

        if(stan == 1)
        {
            zmien_napis_statusu(NAGRYWANIE);
        }
        else
        {
            ui->list_przebiegow->addItem("Nagranie nr " + QString::number(robot->getRamieLastId()));
            zmien_napis_statusu( GOTOWY );
        }

}

void MainWindow::on_rec_play_clicked()
{
    zmien_napis_statusu( ODTWARZANIE );
    int pos = ui->list_przebiegow->currentRow();
    if(pos<0)
    robot->odtworz(0);
    else
    robot->odtworz(pos);
}

void MainWindow::on_list_przebiegow_itemDoubleClicked(QListWidgetItem *item)
{
    (void)item;
    on_rec_play_clicked();
}

void MainWindow::koniec_odtwarzania()
{
    zmien_napis_statusu( GOTOWY );
}

void MainWindow::zmien_napis_statusu(MainWindow::status_nagrywania status)
{
    switch( status )
    {
    case ODTWARZANIE:
        ui->rec_info->setText("Odtwarzanie...");
        ui->rec_info->setPalette(kol_blue);
        break;

    case GOTOWY:
        ui->rec_info->setText("Gotowe do otworzenia");
        ui->rec_info->setPalette(kol_green);
        break;

    case NAGRYWANIE:
        ui->rec_info->setText("Nagrywanie ekranu");
        ui->rec_info->setPalette(kol_red);
        break;
    }
}
