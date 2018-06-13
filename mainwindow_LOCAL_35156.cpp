#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QGraphicsItem>
#include <QTime>

#include <qmath.h>

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

    rect = s->sceneRect();

    polacz_sygnaly();

    bck.load(":/tlo.png");
    tlo = bck.scaled(s->width(),s->height());
    s->addPixmap(tlo);

    dodaj_obiekty_fizyczne();
    wstepne_kolory_labeli();

    trzymany = NULL;
    spadanie->zarejestruj_obiekty( &klocki );

    robot->ustaw( QPoint( scena->width()/2-100, scena->height()/2-200 ) );

    timer_zegarka.start(500);

    robot->dodaj_strefe_zakazana( QRect( 0, 0, 235, 310) );
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
    s->addPixmap(tlo);

    godz = min = sec = NULL;
    rysuj_wskazowki();

    for( int i=0; i<klocki.size(); i++ )
    {
        s->addItem( klocki[i].wsk );
    }

    QGraphicsItem *it = s->itemAt(p2, QTransform());
    robot->zlap(it);


    narysuj_ramie(p0,p1);
    narysuj_ramie(p1,p2);
}


void MainWindow::narysuj_ramie(QPoint p0, QPoint p1)
{

    int dl_modul = sqrt( (p1.y()-p0.y())*(p1.y()-p0.y()) + (p1.x() - p0.x()) * (p1.x() - p0.x()));
    QPixmap modul(":/modul.png");
    QPoint movedp0;


    int dx = (dl_modul+40)/7;
    int dy = 40;

    movedp0.setY(p0.y()-dy);
    movedp0.setX(p0.x()-dx);

    QSize wymiar ( dl_modul+40, dy*2);
    QPixmap modul2 =  modul.scaled(wymiar,Qt::IgnoreAspectRatio, Qt::FastTransformation);
    QGraphicsPixmapItem *reka = new QGraphicsPixmapItem (modul2);

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
    robot->ustaw_czas_odtwarzania( position );
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

QGraphicsItem * MainWindow::rysuj_wskazowke(QColor kolor, int max, double wartosc, int dlugosc)
{
    int x = 160,  y = 236; // środek tarczy zegara

    if( wartosc > max ) wartosc -= max;

    wartosc -= max/4; // 0 musi być z prawej

    double kat = 2*M_PI - (wartosc/max)*2*M_PI;

    int x_k = x + dlugosc*cos(kat);
    int y_k = y - dlugosc*sin(kat);

    QPen pen(kolor);
    pen.setWidth(2);

    QGraphicsItem *w = s->addLine(x, y, x_k, y_k, pen );

    return w;
}

void MainWindow::usun_wskazowke(QGraphicsItem *w)
{
    if( w != NULL )
    {
        s->removeItem( w );
        delete w;
        w = NULL;
    }
}

void MainWindow::rysuj_wskazowki()
{
    QTime czas = QTime::currentTime();

    usun_wskazowke(godz); usun_wskazowke(min); usun_wskazowke(sec);

    godz = rysuj_wskazowke( Qt::black, 12, czas.hour() + czas.minute()/60.0, 30 );
    min = rysuj_wskazowke( Qt::green, 60, czas.minute() + czas.second()/60.0, 40 );
    sec = rysuj_wskazowke( Qt::blue, 60, czas.second(), 50 );
}

void MainWindow::rysuj_strefe_zakazana(QRect strefa)
{
    s->addRect( strefa, QPen(Qt::red) );
}

void MainWindow::mysza_event(QPoint p)
{
    if( trzymany )
    {
        QPointF pos(p);
        QSizeF rozmiar = trzymany->boundingRect().size();
        QRectF obszar(pos, rozmiar);

        if( spadanie->czy_cos_jest(obszar, trzymany) ) return;
    }

    robot->ustaw(p);
}

void MainWindow::robot_zlapal(QGraphicsItem *obj)
{
    trzymany = obj;
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

void MainWindow::dodaj_obiekty_fizyczne()
{
    QPixmap map(":/kw.png");


    QPixmap polka(":/polka.png");
    QPixmap szafka(":/szafka_mala.png");
    map = map.scaled(50, 50);

    QGraphicsItem * wsk;
    obiekt obj = {NULL, 0, 1};

    for( int i=0; i<4; i++ )
    {
        wsk = s->addPixmap(map);
        wsk->setPos(50+60*i, 500);

        obj.wsk = wsk;
        klocki.push_back(obj);
    }

    for( int i=0; i<3; i++ )
    {
        wsk = s->addPixmap(map);
        wsk->setPos(80+60*i, 400);

        obj.wsk = wsk;
        klocki.push_back(obj);
    }

    obiekt ikea = {NULL, 0 , 0 };
    QPixmap repolka = polka.scaled(350,polka.height()/2);
    wsk = s->addPixmap(repolka);
    wsk->setPos(450,400);
    ikea.wsk = wsk;
    klocki.push_back(ikea);
    robot->dodaj_strefe_zakazana( QRect( ikea.wsk->pos().toPoint(), ikea.wsk->boundingRect().size().toSize() ) );

    QPixmap reszafka = szafka.scaled(270,250);
    wsk = s->addPixmap(reszafka);
    wsk->setPos(25,550);
    ikea.wsk = wsk;
    klocki.push_back(ikea);
    robot->dodaj_strefe_zakazana( QRect( ikea.wsk->pos().toPoint(), ikea.wsk->boundingRect().size().toSize() ) );
}

void MainWindow::wstepne_kolory_labeli()
{
    //------qlabel colors
    ui->rec_info->setAutoFillBackground(true);
    kol_green = ui->rec_info->palette();
    kol_red = ui->rec_info->palette();
    kol_blue = ui->rec_info->palette();
    kol_green.setColor(QPalette::Window, QColor(Qt::green));
    kol_red.setColor(QPalette::Window, QColor(Qt::red));
    kol_blue.setColor(QPalette::Window, QColor::fromRgb(51,255,255));
}

void MainWindow::polacz_sygnaly()
{
    connect(scena, SIGNAL(mysza(QPoint)), this, SLOT(mysza_event(QPoint)));
    connect(robot, SIGNAL(rysuj(QPoint,QPoint,QPoint)), this, SLOT(rysuj(QPoint,QPoint,QPoint)));
    connect(scena, SIGNAL(klawisz(int)), robot, SLOT(KeyEvent(int)));
    connect(robot, SIGNAL(zlapal(QGraphicsItem*)), spadanie, SLOT(zlapane(QGraphicsItem*)));
    connect(robot, SIGNAL(zlapal(QGraphicsItem*)), this, SLOT(robot_zlapal(QGraphicsItem*)));

    connect(robot, SIGNAL(koniec_odtwarzania()), this, SLOT(koniec_odtwarzania()));

    //----nagrywanie pod r
    connect(robot,SIGNAL(nagrywanie(int)),this,SLOT(nagraj(int)));

    connect(&timer_zegarka, SIGNAL(timeout()), this, SLOT(rysuj_wskazowki()));

    connect(robot, SIGNAL(rysuj_strefe_zakazana(QRect)), this, SLOT(rysuj_strefe_zakazana(QRect)));
}
