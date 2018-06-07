#include "fizyka.h"

#include <QDebug>

fizyka::fizyka(int w, int h)
{
    this->w = w;
    this->h = h;

    connect(&timer, SIGNAL(timeout()), this, SLOT(klatka()));
}

void fizyka::zarejestruj_obiekty(QVector<obiekt> *obiekty)
{
    this->obiekty = obiekty;

    if( obiekty != NULL ) timer.start(4);
    else timer.stop();
}

void fizyka::zlapane(QGraphicsItem *klocek)
{
    if( klocek == NULL )
        for( QVector<obiekt>::iterator obj=obiekty->begin(); obj != obiekty->end(); obj++ )
        {
            obj->czy_zaczepiony = 0;
        }
    else
        for( QVector<obiekt>::iterator obj=obiekty->begin(); obj != obiekty->end(); obj++ )
        {
            if( obj->wsk == klocek ) obj->czy_zaczepiony = 1;
        }
}

void fizyka::klatka()
{
    for( QVector<obiekt>::iterator obj=obiekty->begin(); obj != obiekty->end(); obj++ )
    {
        if( obj->czy_grawitacja == 0 || obj->czy_zaczepiony == 1 ) continue;

        QPointF pkt = obj->wsk->pos();
        QRectF bound = obj->wsk->boundingRect();

        QRectF klocek( pkt, bound.size() );

        double nowy = klocek.bottom()+1;

        if( nowy >= h ) continue;

        if( czy_cos_jest(klocek, obj->wsk) ) continue;

        obj->wsk->moveBy(0, 1);
    }
}

bool fizyka::czy_cos_jest(QRectF klocek, QGraphicsItem *opuszczamy)
{
    for( QVector<obiekt>::iterator obj=obiekty->begin(); obj != obiekty->end(); obj++ )
    {
        if( obj->wsk == opuszczamy ) continue;

        QPointF pkt = obj->wsk->pos();
        QRectF bound = obj->wsk->boundingRect();

        QRectF obszar( pkt, bound.size() );

        if(
            klocek.left() < obszar.right() && klocek.right() > obszar.left() &&
            klocek.bottom() >= obszar.top() && klocek.top() < obszar.bottom()
            )
            return 1;
    }

    return 0;
}
