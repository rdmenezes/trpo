#include "collision.h"
#include "arrowpoint.h"
#include "compare.h"

static bool collides1D(float x11, float x12, float x21, float x22)
{
        if (x11>x12) { float tmp=x11; x11=x12; x12=tmp; }
        if (x21>x22) { float tmp=x21; x21=x22; x22=tmp; }
        return (x21<=x12) && (x22>=x11);
}

bool collides(const QRectF & r1,const QRectF & r2)
{
    bool x=collides1D(r1.x(),r1.x()+r1.width(),r2.x(),r2.x()+r2.width());
    bool y=collides1D(r1.y(),r1.y()+r1.height(),r2.y(),r2.y()+r2.height());
    return x && y;
}

bool collides(const QRect & r1,const QRect & r2)
{
    bool x=collides1D(r1.x(),r1.x()+r1.width(),r2.x(),r2.x()+r2.width());
    bool y=collides1D(r1.y(),r1.y()+r1.height(),r2.y(),r2.y()+r2.height());

    return x && y;
}

bool collides(const QRectF & r,const QPoint & p1, const QPoint & p2)
{
    bool x=collides1D(r.x(),r.x()+r.width(),p1.x(),p2.x());
    bool y=collides1D(r.y(),r.y()+r.height(),p1.y(),p2.y());

    return x && y;
}

bool collides(const QRectF & r,const QPointF & p1, const QPointF & p2)
{
    bool x=collides1D(r.x(),r.x()+r.width(),p1.x(),p2.x());
    bool y=collides1D(r.y(),r.y()+r.height(),p1.y(),p2.y());

    return x && y;
}

inline const ArrowPoint * boundaryCollides1D(qreal x1min, qreal x1max,
                                             const ArrowPoint * p1,
                                             const ArrowPoint * p2,
                                             qreal (ArrowPoint::*get)() const)
{
   const ArrowPoint * x2min=p1;
   const ArrowPoint * x2max=p2;
   qreal        x2minval=(p1->*get)();
   qreal        x2maxval=(p2->*get)();

   if (x2minval>x2maxval)
   {
       x2min=p2; x2max=p1;
       x2minval=(p2->*get)(); x2maxval=(p1->*get)();
   }
   const ArrowPoint * result=NULL;
   if (fuzzyCompare(x1min,x2maxval)) result=x2max;
   if (fuzzyCompare(x1max,x2minval)) result=x2min;
   return result;
}
inline bool compareInRange(const ArrowPoint * p, qreal x1min, qreal x1max,
                            qreal (ArrowPoint::*get)() const)
{
  bool result=false;
  if (p)
  {
      if (x1min<=(p->*get)() && x1max>=(p->*get)())
      {
          if (p->isBeginPoint() || p->isEndingPoint())
            result=true;
      }
  }
  return result;
}

bool boundaryCollides(const QRectF & r, const ArrowPoint * p1, const ArrowPoint * p2)
{
  //Determine, whether collision is on x
  const ArrowPoint * tx=boundaryCollides1D(r.left(),r.right(),p1,p2,&ArrowPoint::x);
  bool result=compareInRange(tx,r.top(),r.bottom(),&ArrowPoint::y);
  //Determine, whether collision is on y
  if (!result)
  {
      const ArrowPoint * ty=boundaryCollides1D(r.top(),r.bottom(),p1,p2,&ArrowPoint::y);
      result=compareInRange(ty,r.left(),r.right(),&ArrowPoint::x);
  }
  return result;
}

