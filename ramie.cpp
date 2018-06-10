#include "ramie.h"

ramie::ramie(int l1, int l2, QPoint poczatek)
{
    k = new kinematyka(l1, l2, poczatek);
    p0 = poczatek;

    trzymany = NULL;
    aktualny = docelowy = p0;

    connect(&timer, SIGNAL(timeout()), this, SLOT(animacja()));
    timer.start(3);
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

ramie::~ramie()
{
    delete k;
}

void ramie::ustaw(QPoint p)
{
    docelowy = p;
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
        emit nagrywanie();
        qDebug()<<aktualny;
    }
}

void ramie::animacja()
{
    if( aktualny == docelowy ) return;

    p2 = aktualny = wyznacz_kolejny();
    p1 = k->przelicz(p2);

    if( p1.isNull() ) return;

    p1.setX( p1.x() + p0.x() );
    p1.setY( p0.y() - p1.y() );

    if( trzymany ) trzymany->setPos( p2 );

    emit rysuj(p0, p1, p2);
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
