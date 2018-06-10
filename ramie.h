#ifndef RAMIE_H
#define RAMIE_H

#include <QObject>
#include <QGraphicsItem>
#include <QTimer>

#include "kinematyka.h"

class ramie : public QObject
{
    Q_OBJECT

public:
    ramie( int l1, int l2, QPoint poczatek );
    void zlap( QGraphicsItem * klocek );
    QGraphicsItem * zlapany( void );

    ~ramie( void );

signals:
    void rysuj( QPoint p0, QPoint p1, QPoint p2 );

    void zlapal( QGraphicsItem * klocek );
    void nagrywanie();  //odsyłacz do qLabela na ui

public slots:
    void ustaw( QPoint p );
    void KeyEvent( int kod );

private slots:
    void animacja( void );

private:
    QPoint wyznacz_kolejny( void );

    kinematyka * k;
    QPoint p1, p0, p2;

    QGraphicsItem * trzymany;

    // do animacji - usuwanie teleportowania ramienia po kliknięciu gdzieś dalej
    QPoint aktualny, docelowy;
    QTimer timer;

    //------nagrywanie
    int kamera = -1 ;  //-1  nie nagrywa, 1 nagrywa
    QVector<QPoint> ruch_robota;

};


#endif // RAMIE_H
