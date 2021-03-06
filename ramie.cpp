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

    nr_probki = nr_przebiegu = -1;
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
        if( timer_odtwarzania.isActive() ) timer_odtwarzania.stop();

        nr_przebiegu = numer;
        nr_probki = 0;

        docelowy = zbior_przebiegow[ nr_przebiegu ]->at(0);
    }
}

void ramie::dodaj_strefe_zakazana(QRect strefa)
{
    zakazane.push_back( strefa );
}

int ramie::getRamieLastId()
{
    return zbior_przebiegow.size();
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
    if( timer_odtwarzania.isActive() ) return;

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
        if( trzymany == NULL ) return;

        trzymany->moveBy(0, 1);

        trzymany = NULL;

//        emit rysuj(p0, p1, p2);
        emit zlapal(NULL);
    }

    if (kod == Qt::Key_R)
    {
        kamera*= -1;
        emit nagrywanie(kamera);

        if(kamera == 1)
        {
            ruch_robota = new przebieg;
        }

        else
            zbior_przebiegow.push_back( ruch_robota );
    }
}

void ramie::ustaw_czas_odtwarzania(int czas)
{
    timer_odtwarzania.setInterval( czas );
    timer.setInterval(czas);
}

void ramie::animacja()
{
    if( timer_odtwarzania.isActive() ) return;

    if( aktualny == docelowy )
    {
        if( nr_przebiegu > -1 && timer_odtwarzania.isActive() == 0 ) timer_odtwarzania.start();
        return;
    }

    p2 = wyznacz_kolejny();
    if( p2 == aktualny ) return;

    aktualny = p2;

    ustaw_bez_animacji();

    if(kamera == 1)
    {
        ruch_robota->append(aktualny);
    }
}

void ramie::odtwarzanie()
{
    if( nr_przebiegu == -1 ) return;

    if( nr_przebiegu < zbior_przebiegow.size() )
    {
        przebieg *dane = zbior_przebiegow[nr_przebiegu];

        if( nr_probki < dane->size() )
        {
            p2 = aktualny = dane->at(nr_probki++);

            ustaw_bez_animacji();
        }
        else
        {
            timer_odtwarzania.stop();
            emit koniec_odtwarzania();
            docelowy = aktualny;

            nr_przebiegu = -1;
        }
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

    if( czy_moge_tam_isc(p) == 0 )
    {
        if( trzymany ) trzymany->setPos(aktualny);
        return aktualny;
    }

//    if( trzymany ) trzymany->setPos(p);

    return p;
}

bool ramie::czy_moge_tam_isc(QPoint p)
{
    for( QVector<QRect>::iterator strefa=zakazane.begin(); strefa != zakazane.end(); strefa++ )
    {
        if( strefa->contains( p ) )
        {
            emit rysuj_strefe_zakazana( *strefa );
            return 0;
        }
    }

    return 1;
}

