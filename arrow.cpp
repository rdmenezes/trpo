#include "arrow.h"
#include <algorithm>
#define _USE_MATH_DEFINES
#include <math.h>
#include <QHash>
#include <QPair>


Arrow::Arrow(ObjectConnector * self, Diagram * d, bool tunneled,bool tunneled_end)
      :DiagramObject(ST_LINE)
{
    m_self=self;
    self->setParent(this);

    setDiagram(d);
    m_tunneled_begin=tunneled;
    m_tunneled_end=tunneled_end;

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

void Arrow::setLine(qreal x1, qreal y1, qreal x2, qreal y2)
{
   m_self->setLine(x1,y1,x2,y2);
   regenerate();
}

void Arrow::regenerate()
{
    QRectF rct=sceneDrawingBounds();

    setX(rct.x());
    setY(rct.y());
}

QRectF Arrow::boundingRect() const
{
    QRectF rct=sceneDrawingBounds();
    return QRectF(-A_ROUNDING_PADDING,-A_ROUNDING_PADDING,rct.width()+2*A_ROUNDING_PADDING,
                                                          rct.height()+2*A_ROUNDING_PADDING);
}


#define DEGREE 16

void Arrow::drawBracket(QPainter * p,const QPointF & point ,qreal angle)
{
   double bcos=cos(angle/180.0*M_PI);
   double bsin=sin(angle/180.0*M_PI);
   QPointF part=point-A_ROUNDING_PADDING*QPointF(bcos,-bsin);
   QRectF  rect(part.x()-A_ROUNDING_PADDING,part.y()-A_ROUNDING_PADDING,
                2*A_ROUNDING_PADDING,2*A_ROUNDING_PADDING
               );
    p->drawArc(rect,(angle+30)*DEGREE,120*DEGREE);
    p->drawArc(rect,(angle+210)*DEGREE,120*DEGREE);
}

void Arrow::paint(QPainter * p)
{
    p->setPen(QPen(QBrush(A_COLOR),A_PEN_WIDTH));

    if (m_tunneled_end)
        drawBracket(p,translate(m_self->p2()),m_self->angle());
    if (m_tunneled_begin)
        drawBracket(p,translate(m_self->p1()),m_self->angle()+180);


    //Calculate first point
    QPointF p1=m_self->p1();
    Direction self=m_self->direction();
    bool mustmovefirstpoint=false;
    QVector<ObjectConnector *> inputctrs=m_self->getConnected(0,C_INPUT);
    bool checkDistance=m_self->length()>A_ROUNDING_RADIUS;
    if (inputctrs.size())
    {
        mustmovefirstpoint=true;
        for (int i=0;i<inputctrs.size();i++)
        {
          ObjectConnector * m_input=inputctrs[i];
          qreal distance=QLineF(m_input->p1(),m_self->p1()).length();
          Direction input=m_input->direction();
          if (distance>A_ROUNDING_RADIUS && isNotCollinear(input,self) && checkDistance && m_input->drawRoundings())
          {
            //Actual drawing is done here
            QRectF dp;
            qreal angle=0.0f;
            constructInputRounding(m_input,dp,angle);
            p->drawArc(dp,angle,90*DEGREE);
          } else mustmovefirstpoint=false;
         }
    }
    if (mustmovefirstpoint)
    {
        //Actually in this case we are drawing input rounding
        //Compute draw point
         QPointF points_access[4]={m_self->p1(),m_self->p1(),m_self->p1(),m_self->p1()};
         points_access[D_RIGHT]+=QPointF(A_ROUNDING_RADIUS,0);
         points_access[D_LEFT]+=QPointF(- A_ROUNDING_RADIUS,0);
         points_access[D_TOP]+=QPointF(0,- A_ROUNDING_RADIUS);
         points_access[D_BOTTOM]+=QPointF(0,+ A_ROUNDING_RADIUS);
         p1=points_access[m_self->direction()];
    }

    //Calculate second point
    QPointF p2=m_self->p2();
    QVector<ObjectConnector *> outputctrs=m_self->getConnected(1,C_OUTPUT);
    bool mustmovesecondpoint=false;
    if (outputctrs.size())
    {
        mustmovesecondpoint=true;
        for (int i=0;i<outputctrs.size();i++)
        {
         ObjectConnector * m_output=outputctrs[i];
         qreal distance=QLineF(m_output->p2(),m_self->p2()).length();
         if (distance<A_ROUNDING_RADIUS || !isNotCollinear(m_output->direction(),self)
                                        || !checkDistance
                                        || !m_output->drawRoundings())
            mustmovesecondpoint=false;
        }
    }
    if (mustmovesecondpoint)
    {
        QPointF points_access[4]={m_self->p2(),m_self->p2(),m_self->p2(),m_self->p2()};
        points_access[D_RIGHT]+=QPointF(-A_ROUNDING_RADIUS,0);
        points_access[D_LEFT]+=QPointF(+ A_ROUNDING_RADIUS,0);
        points_access[D_TOP]+=QPointF(0,+ A_ROUNDING_RADIUS);
        points_access[D_BOTTOM]+=QPointF(0,- A_ROUNDING_RADIUS);
        p2=points_access[self];
    }
    p->drawLine(translate(p1),translate(p2));
    bool drawTriangle=false;
    for (int i=0;i<outputctrs.size();i++)
        drawTriangle=drawTriangle || outputctrs[i]->drawTriangle();
    //Draw an arrow
    if (!(outputctrs.size()) || drawTriangle )
    {
        QPointF pts[3]={m_self->p2(),m_self->p2(),m_self->p2()};
        QLineF  m_pvec=m_self->unitVector();
        QLineF  m_normal=m_self->normalVector().unitVector();
        pts[2]=pts[1]-=QPointF(m_pvec.dx(),m_pvec.dy())*A_ARROW_HORIZ_LEN;
        pts[1]+=QPointF(m_normal.dx(),m_normal.dy())*A_ARROW_VERT_LEN;
        pts[2]-=QPointF(m_normal.dx(),m_normal.dy())*A_ARROW_VERT_LEN;
        QBrush oldbrush=p->brush();
        p->setBrush(A_COLOR);
        pts[0]=translate(pts[0]);
        pts[1]=translate(pts[1]);
        pts[2]=translate(pts[2]);
        p->drawPolygon(pts,3);
        p->setBrush(oldbrush);
    }
}

typedef QPair<Direction,Direction> DirectionPair;
typedef QPair<QPointF,qreal>       InputPair;

void Arrow::constructInputRounding(ObjectConnector * input,QRectF & p1, qreal & angle)
{

  QHash< QPair<Direction,Direction>,QPair<QPointF,qreal> > points;
  points.insert(DirectionPair(D_BOTTOM,D_RIGHT),
                InputPair(translate(m_self->p1())+QPointF(A_ROUNDING_RADIUS,-A_ROUNDING_RADIUS),180*DEGREE)
               );
  points.insert(DirectionPair(D_TOP,D_RIGHT),
                InputPair(translate(m_self->p1())+QPointF(A_ROUNDING_RADIUS,A_ROUNDING_RADIUS),90*DEGREE)
               );
  points.insert(DirectionPair(D_BOTTOM,D_LEFT),
                InputPair(translate(m_self->p1())+QPointF(- A_ROUNDING_RADIUS, - A_ROUNDING_RADIUS),270*DEGREE)
               );
  points.insert(DirectionPair(D_TOP,D_LEFT),
                InputPair(translate(m_self->p1())+QPointF(- A_ROUNDING_RADIUS, A_ROUNDING_RADIUS),0*DEGREE)
               );

  points.insert(DirectionPair(D_RIGHT,D_TOP),
                InputPair(translate(m_self->p1())+QPointF(- A_ROUNDING_RADIUS,- A_ROUNDING_RADIUS),270*DEGREE)
               );
  points.insert(DirectionPair(D_LEFT,D_TOP),
                InputPair(translate(m_self->p1())+QPointF(A_ROUNDING_RADIUS,- A_ROUNDING_RADIUS),180*DEGREE)
               );
  points.insert(DirectionPair(D_RIGHT,D_BOTTOM),
                InputPair(translate(m_self->p1())+QPointF(- A_ROUNDING_RADIUS,A_ROUNDING_RADIUS),0*DEGREE)
               );
  points.insert(DirectionPair(D_LEFT,D_BOTTOM),
                InputPair(translate(m_self->p1())+QPointF(A_ROUNDING_RADIUS,A_ROUNDING_RADIUS),90*DEGREE)
               );
  // Uncomment this for debug
  //Direction inpd=input->direction();
  //Direction seld=m_self->direction();
  InputPair  res=points[DirectionPair(input->direction(),m_self->direction())];
  p1=QRectF(
             res.first-QPointF(A_ROUNDING_RADIUS,A_ROUNDING_RADIUS),
             res.first+QPointF(A_ROUNDING_RADIUS,A_ROUNDING_RADIUS)
           );
  angle=res.second;
}

Arrow::~Arrow()
{
 delete m_self;
}

bool Arrow::drawRoundings() const
{
  return true;
}

int Arrow::type() const
{
  return IsArrow;
}

QLineF  Arrow::collisionLine() const
{
    return QLineF(*m_self);
}


DiagramObject * Arrow::clone()
{
    return new Arrow(new ObjectConnector(*m_self),this->diagram(),m_tunneled_begin,m_tunneled_end);
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



