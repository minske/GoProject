#ifndef PIERRE_H
#define PIERRE_H
#include <QtGUI>
#include <QRect>
#include "partie.h"
#define E 32

class Pierre
{
public :
    Pierre(const coup* c);
    const coup* getCoup() const {return corres;}
    const QRect& getRect() const {return rect;}
    void setEllipse(QGraphicsPixmapItem* el);
    QGraphicsPixmapItem* getEllipse() const {return ellipse;}
    ~Pierre() { delete ellipse;}

private :
    QRect rect;
    const coup* corres;
    QGraphicsPixmapItem* ellipse;
};

#endif // PIERRE_H
