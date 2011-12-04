#include "box.h"
#include "diagramscene.h"
#include "keytest.h"
#include "objecttexteditor.h"
#include "arrowpoint.h"
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

void Box::save(QDomDocument * /* doc */,
               QDomElement *  /* element */)
{
    //!< TODO: Implement this later
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
    this->scene()->update();
}

void Box::updateString(const QString & text)
{
   if (!text.isEmpty())
   {
        m_real_text=text;
        m_view_text=text;
        regenerate();
        update();
   }
}

/*
void Box::regenerate()
{

   QRectF       numrect=static_cast<DiagramScene *>(this->scene())->getDefaultBlockNumberSize();
   QRect        rect(m_rect.x(),m_rect.y(),m_rect.width(),m_rect.height());
   QFontMetrics metrics(scene()->font());
   QRect basic_rect=metrics.boundingRect(rect,Qt::AlignCenter,m_real_text);
   //printf("%d %d %d %d\n",basic_rect.x(),basic_rect.y(),basic_rect.width(),basic_rect.height());
   m_number_pos.setX(m_rect.x()+m_rect.width()-numrect.width());
   m_number_pos.setY(m_rect.y()+m_rect.height());
   if (basic_rect.width()<=m_rect.width() &&
       basic_rect.height()<=m_rect.height()-numrect.height()
       )
   {
     m_view_text=m_real_text;
   }
   else
   {
       //printf("Worked other lap");
       QStringList rows=m_real_text.split("\n",
                                            QString::KeepEmptyParts,
                                            Qt::CaseSensitive);
       m_viewed_string="";
       bool exit=false;
       int i=0;
       for (i=0;(i<rows.size()) && !exit;i++)
       {
         QString old_v_string=m_viewed_string;        
         QString row_string_elided=metrics.elidedText(rows[i],Qt::ElideRight,
                                                      m_rect.width());
         if (metrics.boundingRect(rect,Qt::AlignCenter,row_string_elided).width()>m_rect.width())
             row_string_elided="";
         //Add new string
         m_viewed_string+=row_string_elided;
         m_viewed_string+="\n";
         if (metrics.boundingRect(rect,Qt::AlignCenter,m_viewed_string).height()
             >
             m_rect.height()-numrect.height())
         {
             m_viewed_string=old_v_string;
             exit=true;
             if (i!=0)
                 rows[i]="...";
             else
                 rows[i]="";
         }
         else rows[i]=row_string_elided;
       }
       printf("%s",rows[0].toStdString().c_str());
       m_viewed_string=rows[0];
       if (i!=1) m_viewed_string+="\n";
       for (int j=1;j<i;j++)
       {
           m_viewed_string+=rows[j];
           if (j!=i-1)
                m_viewed_string+="\n";
       }
   }
   m_string_pos=metrics.boundingRect(m_rect.x(),m_rect.y()+1,m_rect.width(),
                                     m_rect.height()-numrect.height(),
                                     Qt::AlignCenter,m_viewed_string);

}
*/
/*
void Box::setRect(const QRectF & rect)
{
    //m_rect=rect;
    regenerate();
}
*/

void Box::clearPointReferences()
{
  for (int i=0;i<BLOCK_SIDES;i++)
  {
    for (int j=0;j<MAX_LINE_REFERENCES;j++)
    {
      if (m_line_refs[i][j])
      {
       m_line_refs[i][j]->deattachFromBlock();
       m_line_refs[i][j]=NULL;
      }
    }
  }
}

BoxSide Box::sideOfPoint(ArrowPoint * point, const QRectF & m_rect)
{
 QPointF center=m_rect.center();
 qreal dx=point->x()-center.x();
 qreal dy=point->y()-center.y();
 qreal fdx=fabs(dx);
 qreal fdy=fabs(dy);
 BoxSide result=BIS_BOTTOM;
 if (fdx>=fdy) {  if (dx<0) result=BIS_LEFT; else result=BIS_RIGHT;  }
 else          {  if (dy<0) return result=BIS_TOP;   }
 return result;
}

BoxSide Box::sideOfPoint(ArrowPoint *point)
{
    return BIS_BOTTOM;
    //return sideOfPoint(point,m_rect);
}

void Box::removePointReference(ArrowPoint * point)
{
    for (int i=0;i<BLOCK_SIDES;i++)
    {
      for (int j=0;j<MAX_LINE_REFERENCES;j++)
      {
         if (m_line_refs[i][j]==point)
               m_line_refs[i][j]=NULL;
      }
    }
}

void Box::addPointReference(ArrowPoint * point)
{
    BoxSide bis=sideOfPoint(point);
    bool handled=false;
    for (int j=0;(j<MAX_LINE_REFERENCES) && (!handled); j++)
    {
        if (m_line_refs[bis][j]==NULL)
        {
            m_line_refs[bis][j]=point;
            handled=true;
        }
    }
    if (handled)
        point->attachBlock(this);
}


bool Box::canAddToSide(BoxSide side)
{
  int cnt=0;
  for (int i=0;i<MAX_LINE_REFERENCES;i++) if (m_line_refs[side][i]!=NULL) ++cnt;
  return cnt!=MAX_LINE_REFERENCES;
}

bool Box::canAddPointReference(ArrowPoint * point,
                                   BlockEnteringType enter)
{
  BoxSide bis=sideOfPoint(point);
  if (bis==BIS_LEFT || bis==BIS_TOP) return enter==BET_INPUT && canAddToSide(bis);
  if (bis==BIS_BOTTOM)               return canAddToSide(bis);
  if (bis==BIS_RIGHT)                return enter==BET_OUTPUT && canAddToSide(bis);
  return false;
}

MoveRange Box::getRange(ArrowPoint * point)
{
  /*
  BoxSide bis=sideOfPoint(point);
  if (bis==BIS_LEFT)
      return createVerticalRange(m_rect.top(),m_rect.bottom(),m_rect.left());
  if (bis==BIS_RIGHT)
      return createVerticalRange(m_rect.top(),m_rect.bottom(),m_rect.right());
  if (bis==BIS_TOP)
      return createHorizontalRange(m_rect.left(),m_rect.right(),m_rect.top());
  */
  return MoveRange(0,0,0,0);
  //return createHorizontalRange(m_rect.left(),m_rect.right(),m_rect.bottom());
}

void Box::attachAllPoints(const QVector<ArrowPoint *> pts)
{
  /*
  for (int i=0;i<pts.size();i++)
  {
      BoxSide bis=sideOfPoint(pts[i]);
      if (bis==BIS_LEFT) pts[i]->setX(m_rect.left());
      if (bis==BIS_RIGHT) pts[i]->setX(m_rect.right());
      if (bis==BIS_TOP)    pts[i]->setY(m_rect.top());
      if (bis==BIS_BOTTOM) pts[i]->setY(m_rect.bottom());
      pts[i]->update();
      addPointReference(pts[i]);
  }
  */
}
