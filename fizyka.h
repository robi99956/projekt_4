#ifndef FIZYKA_H
#define FIZYKA_H

#include <QGraphicsItem>
#include <QTimer>
#include <QObject>

struct obiekt
{
    QGraphicsItem *wsk;
    bool czy_zaczepiony;
    bool czy_grawitacja;
};

class fizyka : public QObject
{
    Q_OBJECT

public:
    fizyka( int w, int h );
    void zarejestruj_obiekty( QVector< obiekt > *obiekty );

private slots:
    void klatka( void );

private:
    bool czy_cos_jest( QRectF obszar, QGraphicsItem *opuszczamy );
    void zaczep( QPoint pkt );

    QVector< obiekt > *obiekty;
    int w, h;
    QTimer timer;
};

#endif // FIZYKA_H