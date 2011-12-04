#include "commentline.h"
#include "diagram.h"
#include "arrowsegment.h"
#include "arrowpoint.h"
#include <QGraphicsScene>
#include <QPainter>
#include <algorithm>


CommentLine::CommentLine(const QPointF & in,
                         const QPointF & out,
                         ObjectConnector * oin,
                         AttachedComment * attach,
                         Diagram * d) : DiagramObject(ST_LINE)
{
    setDiagram(d);

    m_in=in;
    m_out=out;
    m_self=new ObjectConnector();
    if (oin)
        m_self->addConnector(oin,0,C_INPUT);
    m_parentcomment=attach;

    qreal x=std::min(in.x(),out.x());
    qreal y=std::min(in.y(),out.y());

    setX(x);
    setY(y);
}

bool CommentLine::hasInput() const
 {
     QVector<ObjectConnector *> c=m_self->getConnected(0,C_INPUT);
     return c.size()!=0;
 }
qreal CommentLine::getNormalArcHeight() const
{
   qreal distance=QLineF(m_in,m_out).length();
   if (distance<30) return distance/6;
   else             return 5;
}

QRectF CommentLine::boundingRect() const
{
    QLineF line(translate(m_in),translate(m_out));
    QLineF normal=line.normalVector().unitVector();
    QPointF p(normal.dx(),normal.dy());

    qreal archeight=getNormalArcHeight();
    QPointF pts[4]={ line.p1()+p*archeight,
                     line.p1()-p*archeight,
                     line.p2()+p*archeight,
                     line.p2()-p*archeight,
                   };
    return bound(pts,4);
}

CommentLine::~CommentLine()
{
  delete m_self;
}

void CommentLine::drawCubicCurve(QPointF  p[4],
                                 double t0, double t1,
                                 QPainterPath & path)
{
    double t01=1.0-t0,t11=1.0-t1;
    double a0=t01*t01*t01,a1=3*t0*t01*t01,a2=3*t0*t0*t01,a3=t0*t0*t0;
    double b0=t11*t11*t11,b1=3*t1*t11*t11,b2=3*t1*t1*t11,b3=t1*t1*t1;
    QPointF F0=p[1]-p[0]*a0-p[3]*a3;
    QPointF F1=p[2]-p[0]*b0-p[3]*b3;
    double D=a1*b2-a2*b1;
    QPointF D1=F0*b2-F1*a2;
    QPointF D2=F1*a1-F0*b1;
    D1/=D;
    D2/=D;
    path.moveTo(p[0]);
    path.cubicTo(D1,D2,p[3]);
}

void CommentLine::drawArcs(QPainterPath & path,qreal l, qreal x,qreal hx)
{
    QLineF lineVector=QLineF(translate(m_in),translate(m_out)).unitVector();
    QLineF normalLine=lineVector.normalVector();

    QPointF line(lineVector.dx(),lineVector.dy());
    QPointF normal(normalLine.dx(),normalLine.dy());

    QPointF  p0[4];
    p0[0]=translate(m_in);
    p0[1]=p0[0]+line*l/2+normal*x;
    p0[2]=p0[0]+line*(l+hx)/2+normal*x/2;
    QPointF  middle=p0[0]+line*l/2;
    p0[3]=middle;

    drawCubicCurve(p0,0.4,0.75,path);

    QPointF p1[4];
    p1[0]=translate(m_out);
    p1[1]=p0[0]+line*l/2-normal*x;
    p1[2]=p0[0]+line*(l-hx)/2-normal*x/2;
    p1[3]=middle;
    drawCubicCurve(p1,0.4,0.75,path);
}

void CommentLine::paint(QPainter * p)
{
    QPainterPath path;
    qreal x=this->getNormalArcHeight();
    qreal angle=QLineF(m_in,m_out).angle();
    qreal length=QLineF(m_in,m_out).length();
    if (angle>275  || angle <= 95)
        drawArcs(path,length,x,x);
    else
        drawArcs(path,length,-1*x,x);
    p->setPen(QPen(QBrush(CL_COLOR),CL_WIDTH));
    p->drawPath(path);
}

/*! Returns a type of segment
 */
int CommentLine::type() const
{
    return  IsCommentLine;
}

QLineF CommentLine::collisionLine() const
{
    return QLineF(m_in,m_out);
}

void CommentLine::save(QDomDocument * /* doc */,
               QDomElement *  /* element */)
{
    //!< TODO: Implement this later
}

void CommentLine::load(QDomElement * /* element */,
               QMap<void *, Serializable *> & /* addressMap */ )
{
    //!< TODO: Implement this later
}

void CommentLine::resolvePointers(QMap<void *, Serializable *> &
                          /* adressMap */)
{
    //!< TODO: Implement this later
}



CommentLine::CommentLine(const QPointF & bindedpoint,const QPointF & freepoint) : DiagramObject(ST_LINE)
{
    m_bindedpoint=new QPointF(bindedpoint);
    m_freepoint=freepoint;
    m_isbindedtoline=false;
}

CommentLine::CommentLine(QPointF * bindedpoint,const QPointF & freepoint) : DiagramObject(ST_LINE)
{
  m_bindedpoint=bindedpoint;
  m_freepoint=freepoint;
  static_cast<ArrowPoint *>(bindedpoint)->attachAnnotation(this);
  m_isbindedtoline=true;
}

void CommentLine::deattachFromLine()
{
  if (m_isbindedtoline)
  {
      ArrowPoint * p=static_cast<ArrowPoint *>(m_bindedpoint);
      m_bindedpoint=new QPointF(m_bindedpoint->x(),m_bindedpoint->y());
      m_isbindedtoline=false;
      p->removeAnnotation(this);
  }
}



void CommentLine::die()
{
    m_diagram->removeAnnotationLine(this);
    this->scene()->removeItem(this);
}

bool CommentLine::contains ( const QPointF & point ) const
{
    QLineF unitVector=QLineF(m_in,m_out).unitVector();
    QLineF normalVector=unitVector.normalVector();
    QPointF unit(unitVector.dx(),unitVector.dy());
    QPointF normal(normalVector.dx(),normalVector.dy());

    QPointF p1=translate(m_in);
    QPointF p2=translate(m_out);

    qreal ht=getNormalArcHeight();
    qreal minh,maxh,minv,maxv;
    {
      QPointF p[4]={p1-ht*normal,p1+ht*normal,p2-ht*normal,p2+ht*normal};
      for (int i=0;i<4;i++)
      {
        qreal hps=p[i].x()*unit.x()+p[i].y()*unit.y();
        qreal vps=p[i].x()*normal.x()+p[i].y()*normal.y();
        if (hps>maxh || i==0 ) maxh=hps;
        if (hps<minh || i==0 ) minh=hps;
        if (vps>maxv || i==0 ) maxv=vps;
        if (vps<minv || i==0 ) minv=vps;
      }
    }

    qreal hp=point.x()*unit.x()+point.y()*unit.y();
    qreal vp=point.x()*normal.x()+point.y()*normal.y();
    bool hh=(minh<=hp && hp<=maxh);
    bool vh=(minv<=vp && vp<=maxv);
    return hh && vh;
}


DiagramObject * CommentLine::clone()
{
    return new CommentLine(m_in,m_out,NULL,NULL,this->diagram());
}

