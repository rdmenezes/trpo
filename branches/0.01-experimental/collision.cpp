#include "collision.h"

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



