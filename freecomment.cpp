#include "freecomment.h"
#include "attachedcomment.h"
#include "arrow.h"
#include "objecttexteditor.h"
#include "diagramscene.h"
#include "diagram.h"
#include "keytest.h"
#include <QPainter>
#include <QGraphicsProxyWidget>
#include <QFontMetricsF>

FreeComment::FreeComment(const QPointF & p, Diagram * d, const QString & txt)
            :DiagramObject(ST_RECTANGLE)
{
   m_parentcomment=NULL;
   setDiagram(d);
   DiagramScene * scene=d->scene();

   QFont font=scene->font();
   font.setPointSize(FC_FONT_SIZE);

   QFontMetricsF metrics(font);

   m_text=txt;
   QRectF rect=metrics.boundingRect(METRICS_TEST_RECT,Qt::AlignCenter,m_text);
   m_size.setWidth(rect.width()+2*FC_PADDING);
   m_size.setHeight(rect.height()+2*FC_PADDING);
   setX(p.x()-m_size.width()/2);
   setY(p.y()-m_size.height()/2);

}

QRectF FreeComment::boundingRect() const
{
  return QRectF(0,0,m_size.width(),m_size.height());
}

void FreeComment::paint(QPainter *p)
{
    p->drawText(boundingRect(),Qt::AlignCenter,m_text);
}


QRectF FreeComment::computeRect(const QString & text)
{
    QGraphicsScene * scene=this->scene();

    QFont font=scene->font();
    font.setPointSize(FC_FONT_SIZE);

    QFontMetricsF metrics(font);

    QRectF rect;
    rect  =metrics.boundingRect(METRICS_TEST_RECT,Qt::AlignCenter,text);
    rect.setWidth(rect.width()+2*FC_PADDING);
    rect.setHeight(rect.height()+2*FC_PADDING);
    rect.moveLeft(x()+m_size.width()/2-rect.width()/2);
    rect.moveTop(y()+m_size.height()/2-rect.height()/2);

    return rect;
}

void FreeComment::setText(const QString &text)
{
    QRectF oldrect=collisionRect();
    QString oldtext=m_text;

    QRectF rect=computeRect(text);
    setX(rect.x());
    setY(rect.y());
    m_size=rect.size();
    m_text=text;

    //Tests, whether we can place a free comment
    QVector<CollisionObject *> exc; exc<<this;
    if (parentComment()->line()) exc<<parentComment()->line();
    bool canPlaceFreeComment=this->diagram()->canPlace(this,QVector<int>(),exc);

    //Save old position of line
    CommentLine * line=NULL;
    QPointF       line_old_point;
    if (parentComment()->line())
    {
      line=parentComment()->line();
      line_old_point=line->out();

      //Compute old point position
      Direction side=getSide(oldrect,line_old_point);
      QLineF oldline=getLineBySide(oldrect,side);
      qreal  posx=position(oldline,line_old_point);

      //Compute and set new position
      QPointF newpoint=position(getLineBySide(rect,side),posx);
      line->setLine(line->in().x(),line->in().y(),newpoint.x(),newpoint.y());

      //Check for collisions
       QVector<CollisionObject *> clexc; clexc<<this<<line;
       QVector<int>   clexctypes; clexctypes<<IsCommentLine<<IsArrow;
       DiagramObject * obj=line->getInputObject();
       if (obj) clexc<<obj;
       bool canPlaceCommentLine=this->diagram()->canPlace(line,clexctypes,clexc);
       canPlaceFreeComment=canPlaceFreeComment && canPlaceCommentLine;
    }

    //Revert any changes
    if (! canPlaceFreeComment)
    {
        setX(oldrect.x());
        setY(oldrect.y());
        m_size=oldrect.size();
        m_text=oldtext;
        if (line)
            line->setLine(line->in().x(),line->in().y(),line_old_point.x(),
                                                        line_old_point.y());
    }
    this->scene()->update();
}

void FreeComment::moveTo(const QPointF &p)
{
   setX(p.x()-m_size.width()/2);
   setY(p.y()-m_size.height()/2);
   this->scene()->update();
}

void FreeComment::save(QDomDocument * /* doc */,
               QDomElement *  /* element */)
{
    //!< TODO: Implement this later
}

void FreeComment::load(QDomElement * /* element */,
               QMap<void *, Serializable *> & /* addressMap */ )
{
    //!< TODO: Implement this later
}

void FreeComment::resolvePointers(QMap<void *, Serializable *> &
                          /* adressMap */)
{
    //!< TODO: Implement this later
}

int FreeComment::type() const
{
  return IsFreeComment;
}

QRectF  FreeComment::collisionRect() const
{
  return QRectF(pos(),m_size);
}

DiagramObject * FreeComment::clone()
{
    return new FreeComment(this->pos(),this->diagram(),this->m_text);
}

QString  FreeComment::getEditableText() const
{
  if (m_text==FC_TEXT) return QString("");
  return m_text;
}

void FreeComment::setRect(const QRectF & rect)
{
   //m_rect=rect;
}

