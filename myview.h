#ifndef MYGRAPHICSVIEW_H
#define MYGRAPHICSVIEW_H

#include <QGraphicsView>
#include <QMouseEvent>

class myGraphicsView : public QGraphicsView
{
    Q_OBJECT
public:
    myGraphicsView();

    void mousePressEvent( QMouseEvent * e);
    void mouseMoveEvent( QMouseEvent * e);
    void keyPressEvent( QKeyEvent * e);

signals:
    void mysza( QPoint p );
    void klawisz( int kod );
};

#endif // MYGRAPHICSVIEW_H
