#include "myview.h"
#include <QDebug>

myGraphicsView::myGraphicsView()
{

}

void myGraphicsView::mousePressEvent(QMouseEvent *e)
{
//    emit mysza(e->pos());
}

void myGraphicsView::mouseMoveEvent(QMouseEvent *e)
{
    emit mysza( e->pos() );
}

void myGraphicsView::keyPressEvent(QKeyEvent *e)
{
    emit klawisz( e->key() );
}
