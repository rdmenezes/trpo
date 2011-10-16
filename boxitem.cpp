#include "boxitem.h"
#include "diagramscene.h"
#include "keytest.h"
#include "labeledit.h"
#include <QFontMetrics>


BoxItem::BoxItem(const QPointF & pos,DiagramScene * scene)
{
 m_rect=scene->getDefaultBlockSize(pos);
 m_real_string=DEFAULT_BLOCK_TEXT;
 m_viewed_string=DEFAULT_BLOCK_TEXT;
 m_id=scene->diagram()->getBoxID();
 QFontMetrics metrics(scene->font());
 QRect size=metrics.boundingRect(m_real_string);
 m_string_pos.setX(m_rect.center().x()-size.width()/2);
 m_string_pos.setY(m_rect.center().y()+size.height()/4);
 QFont fnt(scene->font());
 fnt.setPointSize(10);
 QFontMetrics metrics2(fnt);
 QRect numbersize=metrics2.boundingRect(QString::number(m_id));
 m_number_pos.setX(m_rect.right()-numbersize.width());
 m_number_pos.setY(m_rect.bottom());
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

}
