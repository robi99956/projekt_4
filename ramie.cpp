#include "ramie.h"

ramie::ramie(int l1, int l2, QPoint poczatek)
{
    k = new kinematyka(l1, l2, poczatek);
    p0 = poczatek;

    trzymany = NULL;
}

void ramie::zlap(QGraphicsItem *klocek)
{
    if( (klocek == NULL && trzymany != NULL) || (klocek != NULL && trzymany == NULL) )
        trzymany = klocek;
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
    p1 = k->przelicz(p);

    if( p1.isNull() ) return;

    p1.setX( p1.x() + p0.x() );
    p1.setY( p0.y() - p1.y() );

<<<<<<< HEAD
    if( trzymany ) trzymany->setPos( p );
    p2 = p;
=======
>>>>>>> 37300d668c2387c7d451fcae67be995812970978

    emit rysuj(p0, p1, p);
}

void ramie::KeyEvent(int kod)
{
    if( kod == Qt::Key_Space ) trzymany = NULL;
    emit rysuj(p0, p1, p2);
}
