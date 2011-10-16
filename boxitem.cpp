#include "boxitem.h"
#include "diagramscene.h"
#include "keytest.h"
#include "labeledit.h"
#include <QFontMetrics>
#include <QRect>

BoxItem::BoxItem(const QPointF & pos,DiagramScene * scene)
{
 m_rect=scene->getDefaultBlockSize(pos);
 m_real_string=DEFAULT_BLOCK_TEXT;
 m_viewed_string=DEFAULT_BLOCK_TEXT;
 m_id=scene->diagram()->getBoxID();
 //Get small label position;
 QRectF numbersize=scene->getDefaultBlockNumberSize();
 m_number_pos.setX(m_rect.right()-numbersize.width());
 m_number_pos.setY(m_rect.bottom());
 //Compute bounding rect
 QFontMetrics metrics(scene->font());
 m_string_pos=metrics.boundingRect(m_rect.x(),m_rect.y()+1,m_rect.width(),
                                   m_rect.height()-numbersize.height(),
                                   Qt::AlignCenter,m_viewed_string);
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
