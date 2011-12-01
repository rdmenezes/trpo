#include "arrow.h"
#include <algorithm>
#define _USE_MATH_DEFINES
#include <math.h>

QRectF bound(QPointF  ps[], int n)
{
    qreal minx=ps[0].x(),miny=ps[0].y(),maxx=ps[0].x(),maxy=ps[0].y();
    for (int i=1;i<n;i++)
    {
        if (ps[i].x()<minx) minx=ps[i].x();
        if (ps[i].y()<miny) miny=ps[i].y();
        if (ps[i].x()>maxx) maxx=ps[i].x();
        if (ps[i].y()>maxy) maxy=ps[i].y();
    }
    return QRectF(minx,miny,maxx-minx,maxy-miny);
}


Arrow::Arrow(ObjectConnector * self, Diagram * d, bool tunneled)
      :DiagramObject(ST_LINE)
{
    m_self=self;
    setDiagram(d);
    m_tunneled=tunneled;
    m_input=NULL;
    m_output=NULL;

    QRectF rct=sceneDrawingBounds();

    setX(rct.x());
    setY(rct.y());
}

QRectF Arrow::sceneDrawingBounds() const
{
    QPointF normal;
    {
     QLineF rnormal=m_self->normalVector().unitVector();
     normal.setX(rnormal.dx());
     normal.setY(rnormal.dy());
    }
    QRectF rct;
    {
     QPointF pa[4]={ m_self->p1()+normal*A_ROUNDING_PADDING,
                     m_self->p1()-normal*A_ROUNDING_PADDING,
                     m_self->p2()+normal*A_ROUNDING_PADDING,
                     m_self->p2()-normal*A_ROUNDING_PADDING
                  };
     rct=bound(pa,4);
    }
    return rct;
}

QRectF Arrow::boundingRect() const
{
    QRectF rct=sceneDrawingBounds();
    return QRectF(0,0,rct.width(),rct.height());
}


#define DEGREE 16

void drawBracket(QPainter * p,const QPointF & point ,qreal angle)
{
   QPointF part=point-A_ROUNDING_PADDING*QPointF(cos(angle),sin(angle));
   QRectF  rect(part.x()-A_ROUNDING_PADDING,part.y()-A_ROUNDING_PADDING,
                2*A_ROUNDING_PADDING,2*A_ROUNDING_PADDING
               );
   p->setPen(QPen(QBrush(A_COLOR),A_PEN_WIDTH));
   p->drawArc(rect,(angle+30)*DEGREE,120*DEGREE);
   p->drawArc(rect,(angle+240)*DEGREE,120*DEGREE);
}

void Arrow::paint(QPainter * p)
{
    Direction direction=m_self->direction();
    if (m_tunneled)
        drawBracket(p,translate(m_self->p2()),m_self->angle());

}

void Arrow::save(QDomDocument * /* doc */,
               QDomElement *  /* element */)
{
    //!< TODO: Implement this later
}

void Arrow::load(QDomElement * /* element */,
               QMap<void *, Serializable *> & /* addressMap */ )
{
    //!< TODO: Implement this later
}

void Arrow::resolvePointers(QMap<void *, Serializable *> &
                          /* adressMap */)
{
    //!< TODO: Implement this later
}



