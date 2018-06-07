#ifndef RAMIE_H
#define RAMIE_H

#include <QObject>
#include <QGraphicsItem>
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

public slots:
    void ustaw( QPoint p );
    void KeyEvent( int kod );

private:
    kinematyka * k;
    QPoint p1, p0, p2;

    QGraphicsItem * trzymany;
};

#endif // RAMIE_H
