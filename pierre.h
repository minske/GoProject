#ifndef PIERRE_H
#define PIERRE_H
#include <QtGUI>
#include <QRect>
#include "partie.h"
#define E 30

class Pierre
{
public :
    Pierre(const coup* c);
    const coup* getCoup() const {return corres;}
    const QRect& getRect() const {return rect;}
    void setEllipse(QGraphicsEllipseItem* el);
    ~Pierre() {}

private :
    QRect rect;
    const coup* corres;
    QGraphicsEllipseItem* ellipse;
};

#endif // PIERRE_H
