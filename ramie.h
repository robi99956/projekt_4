#ifndef RAMIE_H
#define RAMIE_H

#include <QObject>
#include "kinematyka.h"

class ramie : public QObject
{
    Q_OBJECT

public:
    ramie( uint16_t l1, uint16_t l2, QPoint poczatek );
    ~ramie( void );

signals:
    void rysuj( QPoint p0, QPoint p1, QPoint p2 );

public slots:
    void ustaw( QPoint p );

private:
    kinematyka * k;
    QPoint p1, p0;
};

#endif // RAMIE_H
