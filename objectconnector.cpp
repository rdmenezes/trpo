#include "objectconnector.h"
#include <math.h>

Direction ObjectConnector::direction() const
{
   Direction dir=D_LEFT;
    if (fabs(this->p1().x()-this->p2().x())<0.0001)
    {
       if (p1().y()>p2().y()) dir=D_TOP;
       else dir=D_BOTTOM;
    }
    else
    {
        if (p2().x()>p1().x()) dir=D_RIGHT;
    }
    return dir;
}
