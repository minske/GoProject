#ifndef PIERRE_H
#define PIERRE_H
#include <QtGUI>
#include <QRect>
#include "partie.h"
#define E 31
#define R 0.98


class Pierre
{
public :
    Pierre(const Coup* c);
    const Coup* getCoup() const {return corres;}
    const QRect& getRect() const {return rect;}
    void setEllipse(QGraphicsPixmapItem* el);
    QGraphicsPixmapItem* getEllipse() const {return ellipse;}
    ~Pierre() { delete ellipse;}

private :
    QRect rect;
    const Coup* corres;
    QGraphicsPixmapItem* ellipse;
};

#endif // PIERRE_H
