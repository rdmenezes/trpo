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

bool collidesRL1D(const QRectF & r, const QLineF & line, const QPointF & unit)
{
    qreal hps[4]={ r.topLeft().x()*unit.x()+r.topLeft().y()*unit.y(),
                  r.topRight().x()*unit.x()+r.topRight().y()*unit.y(),
                  r.bottomLeft().x()*unit.x()+r.bottomLeft().y()*unit.y(),
                  r.bottomRight().x()*unit.x()+r.bottomRight().y()*unit.y(),
                 };
    qreal min=hps[0];
    qreal max=hps[0];
    for (int i=1;i<4;i++)
    {
       if  (hps[i]>max) max=hps[i];
       if  (hps[i]<min) min=hps[i];
    }
    qreal hx1=line.x1()*unit.x()+line.y1()*unit.y();
    qreal hx2=line.x2()*unit.x()+line.y2()*unit.y();
    bool result=collides1D(min,max,hx1,hx2);
    return  result;
}

void extractVectors(const QLineF & line,QPointF & unit, QPointF & normal)
{
 QLineF unitVector=line.unitVector();
 QLineF normalVector=unitVector.normalVector();

 unit=QPointF(unitVector.dx(),unitVector.dy());
 normal=QPointF(normalVector.dx(),normalVector.dy());
}

bool collides(const QRectF & r, const QLineF & line)
{
    QPointF unit;
    QPointF normal;
    extractVectors(line,unit,normal);

    bool collidesh=collidesRL1D(r,line,unit);
    bool collidesv=collidesRL1D(r,line,normal);

    return collidesh && collidesv;
}
bool collidesLL1D(const QLineF & l1, const QLineF & l2, const QPointF & p)
{
    qreal l11=l1.x1()*p.x()+l1.y1()*p.y();
    qreal l12=l1.x2()*p.x()+l1.y2()*p.y();
    qreal l21=l2.x1()*p.x()+l2.y1()*p.y();
    qreal l22=l2.x2()*p.x()+l2.y2()*p.y();
    return collides1D(l11,l12,l21,l22);
}

bool collides(const QLineF & l1, const QLineF & l2)
{
    QPointF unit1, normal1;
    QPointF unit2, normal2;
    extractVectors(l1,unit1,normal1);
    extractVectors(l2,unit2,normal2);
    bool collides[4]={collidesLL1D(l1,l2,unit1),collidesLL1D(l1,l2,normal1),
                      collidesLL1D(l1,l2,unit2),collidesLL1D(l1,l2,normal2)};
    return collides[0] && collides[1] && collides[2] && collides[3];
}
