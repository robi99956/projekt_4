#include "fizyka.h"

fizyka::fizyka(int w, int h)
{
    this->w = w;
    this->h = h;

    connect(&timer, SIGNAL(timeout()), this, SLOT(klatka()));
}

void fizyka::zarejestruj_obiekty(QVector<obiekt> *obiekty)
{
    this->obiekty = obiekty;

    if( obiekty != NULL ) timer.start(100);
    else timer.stop();
}

void fizyka::klatka()
{
    for( QVector<obiekt>::iterator obj=obiekty->begin(); obj != obiekty->end(); obj++ )
    {
        if( obj->czy_grawitacja == 1 || obj->czy_zaczepiony == 1 ) continue;

        QRectF klocek = obj->wsk->boundingRect();
        double nowy = klocek.bottom()+1;

        if( nowy < h ) klocek.moveBottom( nowy );
        else continue;

        if( czy_cos_jest(klocek, obj->wsk) ) continue;

        obj->wsk->moveBy(0, 1);
    }
}

bool fizyka::czy_cos_jest(QRectF obszar, QGraphicsItem *opuszczamy)
{
    QRectF klocek;

    for( QVector<obiekt>::iterator obj=obiekty->begin(); obj != obiekty->end(); obj++ )
    {
        if( obj->wsk == opuszczamy ) continue;

        klocek = obj->wsk->boundingRect();

        if(
                obszar.contains( klocek.topLeft() ) ||
                obszar.contains( klocek.topRight() ) ||
                obszar.contains( klocek.bottomLeft() ) ||
                obszar.contains( klocek.bottomRight() )
            )
            return 1;
    }

    return 0;
}
