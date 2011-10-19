#include "boxitem.h"
#include "diagramscene.h"
#include "keytest.h"
#include "labeledit.h"
#include "arrowpoint.h"
#include <QFontMetrics>
#include <QRect>
#include <math.h>
#include <string.h>

BoxItem::BoxItem(const QPointF & pos,DiagramScene * scene)
{
 m_rect=scene->getDefaultBlockSize(pos);
 m_real_string=DEFAULT_BLOCK_TEXT;
 m_viewed_string=DEFAULT_BLOCK_TEXT;
 m_id=scene->diagram()->getBoxID();
 m_childdiagram=-1;
 //Get small label position;
 QRectF numbersize=scene->getDefaultBlockNumberSize();
 m_number_pos.setX(m_rect.right()-numbersize.width());
 m_number_pos.setY(m_rect.bottom());
 //Compute bounding rect
 QFontMetrics metrics(scene->font());
 m_string_pos=metrics.boundingRect(m_rect.x(),m_rect.y()+1,m_rect.width(),
                                   m_rect.height()-numbersize.height(),
                                   Qt::AlignCenter,m_viewed_string);
  //Clears point references
  memset(m_line_refs,0,sizeof(ArrowPoint*)*BLOCK_SIDES*MAX_LINE_REFERENCES);
}

QRectF BoxItem::boundingRect() const
{
    return m_rect;
}

void BoxItem::paint(QPainter *painter,
                    const QStyleOptionGraphicsItem *option,
                    QWidget *widget)
{
  painter->setBrush(Qt::NoBrush);
  QPen pen(QColor(0,0,0));
  pen.setWidth(1);
  painter->setPen(pen);
  QRect rect(m_rect.x(),m_rect.y(),m_rect.width(),m_rect.height());
  painter->drawRect(rect);
  QFont oldfont(painter->font());
  QFont newfont(oldfont);
  newfont.setPointSize(10);
  painter->setFont(newfont);
  painter->drawText(m_number_pos,QString::number(m_id));
  painter->setFont(oldfont);
  painter->drawText(m_string_pos,m_viewed_string);
  (void)option,widget;
}

int BoxItem::type() const
{
    return BoxItem::USERTYPE;
}

void BoxItem::keyPressEvent(QKeyEvent *event)
{
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
           LabelEdit  * field=new LabelEdit(myscene,this);
           QRect tmp(m_rect.x(),m_rect.y(),m_rect.width(),m_rect.height());
           field->setGeometry(tmp);
           field->setFont(myscene->font());
           if (m_real_string!=DEFAULT_BLOCK_TEXT)
            field->setPlainText(m_real_string);
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
}

void BoxItem::updateString(const QString & text)
{
   if (!text.isEmpty())
   {
        m_real_string=text;
        m_viewed_string=text;
        regenerate();
        update();
   }
}

void BoxItem::regenerate()
{
   QRectF       numrect=static_cast<DiagramScene *>(this->scene())->getDefaultBlockNumberSize();
   QRect        rect(m_rect.x(),m_rect.y(),m_rect.width(),m_rect.height());
   QFontMetrics metrics(scene()->font());
   QRect basic_rect=metrics.boundingRect(rect,Qt::AlignCenter,m_real_string);
   //printf("%d %d %d %d\n",basic_rect.x(),basic_rect.y(),basic_rect.width(),basic_rect.height());
   m_number_pos.setX(m_rect.x()+m_rect.width()-numrect.width());
   m_number_pos.setY(m_rect.y()+m_rect.height());
   if (basic_rect.width()<=m_rect.width() &&
       basic_rect.height()<=m_rect.height()-numrect.height()
       )
   {
     m_viewed_string=m_real_string;
   }
   else
   {
       //printf("Worked other lap");
       QStringList rows=m_real_string.split("\n",
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

void BoxItem::setRect(const QRectF & rect)
{
    m_rect=rect;
    regenerate();
}

void BoxItem::clearPointReferences()
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

BoxItemSide BoxItem::sideOfPoint(ArrowPoint * point)
{
 QPointF center=this->m_rect.center();
 qreal dx=point->x()-center.x();
 qreal dy=point->y()-center.y();
 qreal fdx=fabs(dx);
 qreal fdy=fabs(dy);
 BoxItemSide result=BIS_BOTTOM;
 if (fdx>=fdy) {  if (dx<0) result=BIS_LEFT; else result=BIS_RIGHT;  }
 else          {  if (dy<0) return result=BIS_TOP;   }
 return result;
}

void BoxItem::removePointReference(ArrowPoint * point)
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

void BoxItem::addPointReference(ArrowPoint * point)
{
    BoxItemSide bis=sideOfPoint(point);
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

bool BoxItem::canAddToSide(BoxItemSide side)
{
  int cnt=0;
  for (int i=0;i<MAX_LINE_REFERENCES;i++) if (m_line_refs[side][i]!=NULL) ++cnt;
  return cnt!=MAX_LINE_REFERENCES;
}

bool BoxItem::canAddPointReference(ArrowPoint * point,
                                   BlockEnteringType enter)
{
  BoxItemSide bis=sideOfPoint(point);
  if (bis==BIS_LEFT || bis==BIS_TOP) return enter==BET_INPUT && canAddToSide(bis);
  if (bis==BIS_BOTTOM)               return canAddToSide(bis);
  if (bis==BIS_RIGHT)                return enter==BET_OUTPUT && canAddToSide(bis);
  return false;
}
