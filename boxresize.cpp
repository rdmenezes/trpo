#include "selecttool.h"

#define CORNER_SENSIVITY 5

BlockCorner determineCorner(const QPointF & clickpos,const QRectF & r)
{
  QPointF test[4]=       {r.topLeft(),r.topRight(),r.bottomLeft(),r.bottomRight()};
  BlockCorner corners[4]={BC_UPPERLEFT,BC_UPPERRIGHT,BC_LOWERLEFT,BC_LOWERRIGHT};
  for (int i=0;i<4;i++)
  {
      if (fabs(test[i].x()-clickpos.x())<CORNER_SENSIVITY && fabs(test[i].y()-clickpos.y())<CORNER_SENSIVITY )
          return corners[i];
  }
  return BC_CENTER;
}
