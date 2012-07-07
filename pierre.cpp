#include "pierre.h"

Pierre::Pierre(const coup* c)
{
    corres = c;
    rect.setX((c->getAbs()+1)*E-(E*0.45));
    rect.setY((c->getOrd()+1)*E-(E*0.45));
    rect.setWidth(E*0.9);
    rect.setHeight(E*0.9);

}
