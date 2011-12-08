#include "box.h"
#include "commentline.h"
#include "diagramscene.h"
#include "keytest.h"
#include "arrow.h"
#include "objecttexteditor.h"
#include "mainwindow.h"
#include <QFontMetricsF>
#include <math.h>
#include <string.h>
#include <stdio.h>
#include <algorithm>
#include <memory>

Box::Box(const QPointF & p, Diagram * d, const QString & txt)
    :DiagramObject(ST_RECTANGLE)
{
  setDiagram(d);
  DiagramScene * scene=d->scene();
  m_real_text=txt;
  m_view_text=txt;
  m_id=d->getBoxNumber(this);

  m_child=-1;

  std::auto_ptr<QFont>  numberFont(new QFont(scene->font()));
  numberFont->setPointSize(BOX_NUMBER_FONT_SIZE);

  std::auto_ptr<QFont>  textFont(new QFont(scene->font()));
  textFont->setPointSize(BOX_TEXT_FONT_SIZE);

  std::auto_ptr<QFontMetricsF>  numberMetrics(new QFontMetricsF(*numberFont));
  std::auto_ptr<QFontMetricsF>  textMetrics(new QFontMetricsF(*textFont));

  std::auto_ptr<QRectF>  textRect (new QRectF(textMetrics->boundingRect(METRICS_TEST_RECT,Qt::AlignCenter,m_real_text)));
  std::auto_ptr<QRectF>  numberRect(new QRectF(numberMetrics->boundingRect(METRICS_TEST_RECT,
                                                 Qt::AlignCenter,
                                                 BOX_NUMBER_TEXT)));

  qreal width=std::max(numberRect->width(),textRect->width())
              +2*BOX_BORDER_PADDING;
  qreal height=textRect->height()
               +numberRect->height()
               +BOX_DEFAULT_NUMBER_SPACE
               +2*BOX_BORDER_PADDING;
  m_size=QSize(width,height);
  setPos(p-QPointF(width/2,height/2));

  std::auto_ptr<QRectF> bounds(new QRectF(boundingRect()));
  m_number_rect=*numberRect;
  m_number_rect.setLeft(bounds->width()-numberRect->width()
                        -BOX_LABEL_PADDING);
  m_number_rect.setTop(bounds->height()-numberRect->height()
                        -BOX_LABEL_PADDING);

  m_text_rect=*bounds;
  m_text_rect.setTop(BOX_LABEL_PADDING);
  m_text_rect.setBottom(m_text_rect.height()-BOX_LABEL_PADDING);

  m_number_is_visible=true;

  createConnectors();
}


QRectF Box::boundingRect() const
{
    return QRectF(0,0,
                  m_size.width(),m_size.height()
                 );
}


void Box::paint(QPainter * p)
{
 QFont oldfont=this->scene()->font();

 QFont numberFont=this->scene()->font();
 numberFont.setPointSize(BOX_NUMBER_FONT_SIZE);

 QFont textFont=this->scene()->font();
 textFont.setPointSize(BOX_TEXT_FONT_SIZE);

 p->setPen(QPen(QBrush(BOX_LINE_COLOR),BOX_BORDER_WIDTH));

 p->drawRect(boundingRect());

 if (m_number_is_visible)
 {
 p->setPen(QPen(QBrush(BOX_TEXT_COLOR),BOX_BORDER_WIDTH));

 p->setFont(numberFont);
 p->drawText(m_number_rect,
             Qt::AlignLeft | Qt::AlignTop,
             BOX_NUMBER_TEXT);
 }

 p->setFont(textFont);
 p->drawText(m_text_rect,
             Qt::AlignTop | Qt::AlignHCenter,
             m_view_text);

 p->setFont(oldfont);
}

int Box::type() const
{
   return IsBox;
}

QRectF  Box::collisionRect() const
{
    return  QRectF(pos(),m_size);
}

void Box::save(QDomDocument * doc,
               QDomElement * element)
{
    QDomElement box;
    box=doc->createElement("Box");
    QString buf, bufName, bufVal;

    box.setAttribute("real text", m_real_text);    // Real text string
    box.setAttribute("viewed text", m_view_text);  // Viewed text string

    buf=QString("%1 %2 %3 %4").arg(m_text_rect.left()).arg(m_text_rect.right()).arg(m_text_rect.bottom()).arg(m_text_rect.top());
    box.setAttribute("text position", buf);

    buf=QString("%1 %2 %3 %4").arg(m_number_rect.left()).arg(m_number_rect.right()).arg(m_number_rect.bottom()).arg(m_number_rect.top());
    box.setAttribute("number position", buf);

    //buf=::save(m_size);
    box.setAttribute("size of rect", buf);

    box.setAttribute("is visible", m_number_is_visible);
    box.setAttribute("box id", m_id);
    box.setAttribute("location of child", m_child);


    // vector of box connectors
    int j=1;
    for (int i = 0; i < m_connectors.size(); ++i)
    {
        //bufVal=::save(m_connectors.at(i));
        bufName.clear();
        bufName.setNum(j);
        box.setAttribute(bufName.prepend("box connector "), bufVal);
        j++;
        m_connectors.at(i)->save(doc, &box);
    }

    element->appendChild(box);
}

void Box::load(QDomElement * /* element */,
               QMap<void *, Serializable *> & /* addressMap */ )
{
    //!< TODO: Implement this later
}

void Box::resolvePointers(QMap<void *, Serializable *> &
                          /* adressMap */)
{
    //!< TODO: Implement this later
}



void Box::regenerate()
{
 QRectF bounds=boundingRect();
 /* If bounds are smaller than paddings, than don't draw things
  */
 if (bounds.height()<BOX_LABEL_PADDING
     || bounds.width()<BOX_LABEL_PADDING)
 {
     m_view_text="";
     m_number_is_visible=false;
     return;
 }
 bounds.moveLeft(bounds.left()+BOX_LABEL_PADDING);
 bounds.moveRight(bounds.right()-BOX_LABEL_PADDING);
 bounds.moveTop(bounds.top()+BOX_LABEL_PADDING);
 bounds.moveBottom(bounds.bottom()-BOX_LABEL_PADDING);


 QFont numberFont=this->scene()->font();
 numberFont.setPointSize(BOX_NUMBER_FONT_SIZE);

 QFont textFont=this->scene()->font();
 textFont.setPointSize(BOX_TEXT_FONT_SIZE);

 QFontMetricsF numberMetrics(numberFont);
 QFontMetricsF textMetrics(textFont);

 QRectF  numberRect=numberMetrics.boundingRect(METRICS_TEST_RECT,
                                               Qt::AlignCenter,
                                               BOX_NUMBER_TEXT);
 /*! Just because number font is smaller than label
     we can't draw stuff is box smaller
  */
 if (numberRect.width()>bounds.width() ||
     numberRect.height()>bounds.height()
     )
 {
  m_view_text="";
  m_number_is_visible=false;
  return;
 }
 numberRect.moveLeft(bounds.width()-numberRect.width());
 numberRect.moveTop(bounds.height()-numberRect.height());
 m_number_rect=numberRect;
 m_view_text="";
 bool exit=false;
 QStringList rows=m_real_text.split("\n",
                                    QString::KeepEmptyParts,
                                    Qt::CaseSensitive);
 for (int i=0;i<rows.size() && !exit;i++)
 {
     QString oldview=m_view_text;
     QString row_string=textMetrics.elidedText(rows[i],
                                               Qt::ElideRight,
                                               bounds.width()+2*BOX_BORDER_PADDING
                                              );
     m_view_text+=row_string;
     QRectF  oldRect=textMetrics.boundingRect(METRICS_TEST_RECT,Qt::AlignCenter,oldview);
     QRectF  textRect =textMetrics.boundingRect(METRICS_TEST_RECT,Qt::AlignCenter,m_view_text);
     //If current row runs out an object, cut it.
     if (textRect.height()>bounds.height())
     {
        m_view_text=oldview;
        exit=true;
     }
     //If current row is colliding with number, elide it
     else if (bounds.y()+textRect.height()-m_number_rect.y()>0.01)
     {
         exit=true;
         qreal oldwidth=textRect.width();
         qreal newwidth=textRect.width();
         do
         {
             oldwidth=newwidth;
             if (bounds.width()/2+newwidth/2
                 >
                 bounds.width()-m_number_rect.width())
             {
                m_view_text=oldview;
                qreal deltawidth=bounds.width()/(-2)+newwidth/2
                                +m_number_rect.width();
                row_string=textMetrics.elidedText(rows[i],
                                                  Qt::ElideRight,
                                                  newwidth-deltawidth
                                                 );
                m_view_text+=row_string;
                textRect=textMetrics.boundingRect(METRICS_TEST_RECT,Qt::AlignCenter,m_view_text);
                newwidth=textRect.width();
             }
         }
         while(bounds.width()/2+newwidth/2
               >
               bounds.width()-m_number_rect.width()
               &&
               oldwidth-newwidth>0
              );
         m_view_text+="\n";
     } else m_view_text+="\n";
 }
 if (!m_view_text.isEmpty())
     m_view_text.remove(m_view_text.length()-1);
 m_text_rect=bounds;
 m_text_rect.setWidth(m_text_rect.width()+2*BOX_BORDER_PADDING);
}


DiagramObject * Box::clone()
{
    return new Box(this->pos(),this->diagram(),m_real_text);
}

QString  Box::getEditableText() const
{
    if (m_real_text==BOX_DEFAULT_TEXT) return QString("");
    return m_real_text;
}

void Box::createConnectors()
{
  QRectF crect=collisionRect();
  //Top
  m_connectors<<new ObjectConnector(crect.topLeft().x(),
                                    crect.topLeft().y(),
                                    crect.topRight().x(),
                                    crect.topRight().y());
  //Right
  m_connectors<<new ObjectConnector(crect.topRight().x(),
                                    crect.topRight().y(),
                                    crect.bottomRight().x(),
                                    crect.bottomRight().y());
  //Bottom
  m_connectors<<new ObjectConnector(crect.bottomRight().x(),
                                    crect.bottomRight().y(),
                                    crect.bottomLeft().x(),
                                    crect.bottomLeft().y());
  //Left
  m_connectors<<new ObjectConnector(crect.bottomLeft().x(),
                                    crect.bottomLeft().y(),
                                    crect.topLeft().x(),
                                    crect.topLeft().y());
  for (int i=0;i<m_connectors.size();i++)
      m_connectors[i]->setParent(this);

}
bool Box::drawTrianglePart() const
{
    return true;
}
ObjectConnector * Box::getBySide(Direction d)
{
    if (d==D_TOP)
        return m_connectors[0];
    if (d==D_RIGHT)
        return m_connectors[1];
    if (d==D_BOTTOM)
        return m_connectors[2];
    return m_connectors[3];
}
void Box::setPos(const QPointF & ppos)
{
 QPointF old=pos();
 QGraphicsItem::setPos(ppos);
 if (m_connectors.size())
 {
     QPointF delta=ppos-old;
     for (int i=0;i<m_connectors.size();i++)
     {
         m_connectors[i]->setP1(m_connectors[i]->p1()+delta);
         m_connectors[i]->setP2(m_connectors[i]->p2()+delta);
     }
 }
}

void Box::setPos(qreal x, qreal y)
{
  setPos(QPointF(x,y));
}
Box::~Box()
{
  for (int i=0;i<m_connectors.size();i++)
      delete m_connectors[i];
}


void Box::setRect(const QRectF & rect)
{
    setX(rect.x());
    setY(rect.y());
    //setPos(QPointF(rect.x(),rect.y()));
    m_size.setWidth(rect.width());
    m_size.setHeight(rect.height());
    //regenerate();
    this->scene()->update();
}


void Box::setText(const QString & text)
{
    m_real_text=text;
    regenerate();
    this->diagram()->commit();
    this->scene()->update();
}

QPointF getLastCollisionPoint(const QLineF & l ,const QRectF & r, const QPointF & beg)
{
    QPointF  * point=NULL;
    int side=0;
    bool exit=false;
    do
    {
     delete point;
     point=getCollisionPoint(l,r,side);
     ++side;

     exit=point==NULL;
     if (point)
           exit=fabs(beg.x()-point->x())>0.01
             || fabs(beg.y()-point->y())>0.01;
    } while (!exit);
    QPointF result=beg;
    if (point)
    {
        result=*point;
        delete point;
    }
    return result;
}

QPointF Box::receiveCommentLineMove(DiagramObject * obj)
{
   CommentLine * line=static_cast<CommentLine*>(obj);
   line->model()->clear();
   for (int i=0;i<4;i++)
       m_connectors[i]->remove(line->model());
   QLineF testline(line->in(),line->out());
   QPointF   result=getLastCollisionPoint(testline,QRectF(pos(),m_size),line->in());

   Direction clside=getSide(QRectF(pos(),m_size),result);
   ObjectConnector * output=getBySide(clside);
   qreal clpos=position(*output,result);
   output->addConnector(line->model(),clpos,C_OUTPUT,false);
   line->model()->addConnector(output,0,C_INPUT,false);

   Q_ASSERT(line->model()->getConnected(C_INPUT).size());
   static_cast<MainWindow*>(this->scene()->views()[0]->window())->setActionText(QString::number((int)output));

   return result;
}

bool Box::dieIfEqualTo(DiagramObject * o)
{
    if (o!=this)
        return false;

    this->diagram()->removeBoxFromNumbers(this);

    for (int i=0;i<m_connectors.size();i++)
    {
     removeReferencesAndKillLines(m_connectors[i],m_connectors[i],C_INPUT,this->diagram());
     removeReferencesAndKillLines(m_connectors[i],m_connectors[i],C_OUTPUT,this->diagram());
    }
    return true;
}

Direction getSide(const QRectF & r, const QPointF & p)
{
    if (fabs(p.x()-r.left())<0.01) return D_LEFT;
    if (fabs(p.x()-r.right())<0.01) return D_RIGHT;
    if (fabs(p.y()-r.bottom())<0.01) return D_BOTTOM;
    return D_TOP;
}

QLineF    getLineBySide(const QRectF & r, Direction dir)
{
  if (dir==D_LEFT)   return QLineF(r.topLeft(),r.bottomLeft());
  if (dir==D_RIGHT)  return QLineF(r.topRight(),r.bottomRight());
  if (dir==D_TOP)    return QLineF(r.topLeft(),r.topRight());
  return QLineF(r.bottomLeft(),r.bottomRight());
}

