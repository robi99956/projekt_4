#include "ramie.h"

#define TIMER_CZAS 3

ramie::ramie(int l1, int l2, QPoint poczatek)
{
    k = new kinematyka(l1, l2, poczatek);
    p0 = poczatek;

    trzymany = NULL;
    aktualny = docelowy = p0;

    connect(&timer, SIGNAL(timeout()), this, SLOT(animacja()));
    connect(&timer_odtwarzania, SIGNAL(timeout()), this, SLOT(odtwarzanie()));
    timer.start(TIMER_CZAS);
    timer_odtwarzania.setInterval(TIMER_CZAS);

    nr_probki = nr_przebiegu = 0;
}

void ramie::zlap(QGraphicsItem *klocek)
{
    if( (klocek == NULL && trzymany != NULL) || (klocek != NULL && trzymany == NULL) )  
    {
        QSizeF rozmiar = klocek->boundingRect().size();
        if( rozmiar.width() > 100 || rozmiar.height() > 100 ) return;

        trzymany = klocek;
        emit zlapal(klocek);
    }
}

QGraphicsItem *ramie::zlapany()
{
    return trzymany;
}

void ramie::odtworz(int numer)
{
    if( numer < zbior_przebiegow.size() )
    {
        nr_przebiegu = numer;
        nr_probki = 0;
        timer_odtwarzania.start();
    }
}

ramie::~ramie()
{
    delete k;

    for( int i=0; i<zbior_przebiegow.size(); i++ )
    {
        delete zbior_przebiegow[i];
    }
}

void ramie::ustaw(QPoint p)
{
    docelowy = p;
}

void ramie::ustaw_bez_animacji(void)
{
    p1 = k->przelicz(p2);

    if( p1.isNull() ) return;

    p1.setX( p1.x() + p0.x() );
    p1.setY( p0.y() - p1.y() );

    if( trzymany ) trzymany->setPos( p2 );

    emit rysuj(p0, p1, p2);
}

void ramie::KeyEvent(int kod)
{
    if( kod == Qt::Key_Space )
    {
        trzymany->moveBy(0, 1);

        trzymany = NULL;

        emit zlapal(NULL);
        emit rysuj(p0, p1, p2);
    }

    if (kod == Qt::Key_R)
    {
        kamera*= -1;
        emit nagrywanie(kamera);
        if(kamera == 1)
        {
            ruch_robota = new przebieg;
            qDebug()<<aktualny;
        }
        else
            zbior_przebiegow.push_back( ruch_robota );

    }
}

void ramie::ustaw_czas_odtwarzania(int czas)
{
    timer_odtwarzania.setInterval( czas );
}

void ramie::animacja()
{
    if( aktualny == docelowy ) return;

    p2 = aktualny = wyznacz_kolejny();

    ustaw_bez_animacji();

    //-----do nagrywania
//    qDebug()<<aktualny;
    if(kamera == 1)
    {
        ruch_robota->append(aktualny);
    }
}

void ramie::odtwarzanie()
{
    if( nr_przebiegu < zbior_przebiegow.size() )
    {
        przebieg *dane = zbior_przebiegow[nr_przebiegu];

        if( nr_probki < dane->size() )
        {
            p2 = aktualny = dane->at(nr_probki++);

            ustaw_bez_animacji();
        }
        else
            timer_odtwarzania.stop();
    }
}

QPoint ramie::wyznacz_kolejny()
{
    QPoint p = aktualny;

    if( aktualny.x() > docelowy.x() ) p.setX( aktualny.x()-1 );
    else
        if( aktualny.x() < docelowy.x() ) p.setX( aktualny.x()+1 );

    if( aktualny.y() > docelowy.y() ) p.setY( aktualny.y()-1 );
    else
        if( aktualny.y() < docelowy.y() ) p.setY( aktualny.y()+1 );

    return p;
}
