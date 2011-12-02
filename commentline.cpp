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
    m_input=oin;
    m_parentcomment=attach;

    qreal x=std::min(in.x(),out.x());
    qreal y=std::min(in.y(),out.y());

    setX(x);
    setY(y);
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


void CommentLine::drawCubicCurve(const QPointF & p0,
                               const QPointF & p1,
                               const QPointF & p2,
                               const QPointF & p3,
                               double t0, double t1,
                               QPainterPath & path)
{
    double t01=1.0-t0,t11=1.0-t1;
    double a0=t01*t01*t01,a1=3*t0*t01*t01,a2=3*t0*t0*t01,a3=t0*t0*t0;
    double b0=t11*t11*t11,b1=3*t1*t11*t11,b2=3*t1*t1*t11,b3=t1*t1*t1;
    QPointF F0=p1-p0*a0-p3*a3;
    QPointF F1=p2-p0*b0-p3*b3;
    double D=a1*b2-a2*b1;
    QPointF D1=F0*b2-F1*a2;
    QPointF D2=F1*a1-F0*b1;
    D1/=D;
    D2/=D;
    path.moveTo(p0);
    path.cubicTo(D1,D2,p3);
}

void CommentLine::drawArcs(QPainterPath & path,qreal l, qreal x,qreal hx)
{
    QLineF lineVector=QLineF(translate(m_in),translate(m_out)).unitVector();
    QLineF normalLine=lineVector.normalVector();

    QPointF line(lineVector.dx(),lineVector.dy());
    QPointF normal(normalLine.dx(),normalLine.dy());

    QPointF p00=translate(m_in);
    QPointF  p01=p00+line*l/2+normal*x;
    QPointF  p02=p00+line*(l+hx)/2+normal*x/2;

    QPointF  middle=p00+line*l/2;

    drawCubicCurve(p00,p01,p02,middle,0.4,0.75,path);

    QPointF p10=translate(m_out);
    QPointF p11=p00+line*l/2-normal*x;
    QPointF p12=p00+line*(l-hx)/2-normal*x/2;

    drawCubicCurve(p10,p11,p12,middle,0.4,0.75,path);
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

CommentLine::~CommentLine()
{
  if (!m_isbindedtoline)
      delete m_bindedpoint;
}

void CommentLine::die()
{
    m_diagram->removeAnnotationLine(this);
    this->scene()->removeItem(this);
}


/*! Returns a type of segment
 */
int CommentLine::type() const
{
    return CommentLine::USERTYPE;
}

