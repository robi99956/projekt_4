#ifndef RAMIE_H
#define RAMIE_H

#include <QObject>
#include <QGraphicsItem>
#include <QTimer>

#include "kinematyka.h"

typedef QVector< QPoint > przebieg;

class ramie : public QObject
{
    Q_OBJECT

public:
    ramie( int l1, int l2, QPoint poczatek );
    void zlap( QGraphicsItem * klocek );
    QGraphicsItem * zlapany( void );
    void odtworz( int numer );

//    void odtwarzanie(int ktory);

    ~ramie( void );

signals:
    void rysuj( QPoint p0, QPoint p1, QPoint p2 );

    void zlapal( QGraphicsItem * klocek );
    void nagrywanie(int stan);  //odsyłacz do qLabela na ui


public slots:
    void ustaw( QPoint p );
    void ustaw_bez_animacji(void );
    void KeyEvent( int kod );
    void ustaw_czas_odtwarzania( int czas );

private slots:
    void animacja( void );
    void odtwarzanie( void );

private:
    QPoint wyznacz_kolejny( void );

    kinematyka * k;
    QPoint p1, p0, p2;

    QGraphicsItem * trzymany;

    // do animacji - usuwanie teleportowania ramienia po kliknięciu gdzieś dalej
    QPoint aktualny, docelowy;
    QTimer timer;

    //------nagrywanie
    QTimer timer_odtwarzania;

    int kamera = -1 ;  //-1  nie nagrywa, 1 nagrywa
    przebieg *ruch_robota;
    QVector<przebieg*> zbior_przebiegow;
    int nr_przebiegu, nr_probki;
};


#endif // RAMIE_H
