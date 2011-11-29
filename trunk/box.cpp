#include "box.h"
#include "diagramscene.h"
#include "keytest.h"
#include "labeledit.h"
#include "arrowpoint.h"
#include <QFontMetricsF>
#include <math.h>
#include <string.h>
#include <algorithm>

#define METRICS_TEST_RECT  QRectF(0,0,4.0E+9,4.0E+9)


Box::Box(const QPointF & p, Diagram * d, const QString & txt)
    :DiagramObject(ST_RECTANGLE)
{
  setDiagram(d);
  DiagramScene * scene=d->scene();
  m_real_text=txt;
  m_view_text=txt;
  m_id=d->getBoxNumber(this);

  QFont numberFont=scene->font();
  numberFont.setPointSize(BOX_NUMBER_FONT_SIZE);

  QFont textFont=scene->font();
  textFont.setPointSize(BOX_TEXT_FONT_SIZE);

  QFontMetricsF numberMetrics(numberFont);
  QFontMetricsF textMetrics(textFont);

  QRectF  numberRect=numberMetrics.boundingRect(METRICS_TEST_RECT,Qt::AlignCenter,m_real_text);
  QRectF  textRect  =textMetrics.boundingRect(METRICS_TEST_RECT,
                                              Qt::AlignCenter,
                                              BOX_NUMBER_TEXT);

  qreal width=std::max(numberRect.width(),textRect.width())+
              2*BOX_BORDER_WIDTH;
  qreal height=textRect.height()
               +numberRect.height()
               +BOX_DEFAULT_NUMBER_SPACE;
  m_size=QSize(width,height);
  setPos(p-QPointF(width/2,height/2));

  QRectF bounds=boundingRect();
  m_number_rect=numberRect;
  m_number_rect.setLeft(bounds.right()-numberRect.width()
                        -BOX_BORDER_WIDTH);
  m_number_rect.setTop(bounds.bottom()-numberRect.height()
                        -BOX_BORDER_WIDTH);

  m_text_rect=bounds;
  m_text_rect.setLeft(m_text_rect.left()+BOX_BORDER_WIDTH);
  m_text_rect.setRight(m_text_rect.right()-BOX_BORDER_WIDTH);
  m_text_rect.setTop(m_text_rect.top()+BOX_BORDER_WIDTH);
  m_text_rect.setBottom(m_text_rect.bottom()-BOX_BORDER_WIDTH);

}


QRectF Box::boundingRect() const
{
    return QRectF(scenePos(),scenePos()+
                             QPointF(m_size.width(),m_size.height())
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

 this->scene()->setFont(oldfont);
}

int Box::type() const
{
    return Box::USERTYPE;
}

void Box::keyPressEvent(QKeyEvent *event)
{
    /*
    DiagramScene * myscene=static_cast<DiagramScene*>(this->scene());
    if (myscene->isPanelActive())
         return;
    if(event->key()==Qt::Key_Left)
    {
        static_cast<DiagramScene *>(this->scene())->decrementBlockID(this);
    }
    else if (event->key()==Qt::Key_Right)
    {
        static_cast<DiagramScene *>(this->scene())->incrementBlockID(this);
    }
    else
    {
       DiagramScene * myscene=static_cast<DiagramScene *>(this->scene());
       if (isTextEditKey(event) &&
           myscene->editState()==TES_NONE)
       {
           LabelEdit  * field=new LabelEdit(myscene,const_cast<Box*>(this));
           QRect tmp(m_rect.x(),m_rect.y(),m_rect.width(),m_rect.height());
           field->setGeometry(tmp);
           field->setFont(myscene->font());
           if (m_real_text!=DEFAULT_BLOCK_TEXT)
            field->setPlainText(m_real_text);
           //Move to end cursor
           QTextCursor c=field->textCursor();
           c.movePosition(QTextCursor::End);
           field->setTextCursor(c);
           QGraphicsProxyWidget * proxy=this->scene()->addWidget(field);
           myscene->toggleEditStateOn(field,proxy);
           field->grabKeyboard();
           field->keyPressEvent(event);
       }
       else
       {
           this->QGraphicsItem::keyPressEvent(event);
       }
    }
    */
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

void Box::regenerate()
{
   /*
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
   */
}

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
