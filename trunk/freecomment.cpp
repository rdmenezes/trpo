#include "freecomment.h"
#include "objecttexteditor.h"
#include "diagramscene.h"
#include "diagram.h"
#include "keytest.h"
#include <QPainter>
#include <QGraphicsProxyWidget>

FreeComment::FreeComment(const QRectF & rect)
{
    m_rect=rect;
    m_string=DEFAULT_ALABEL_TEXT;
}


QRectF FreeComment::boundingRect() const
{
  return m_rect;
}

void FreeComment::paint(QPainter *painter,
                       const QStyleOptionGraphicsItem *option,
                       QWidget *widget)
{
    painter->drawText(m_rect,m_string);
    (void)option,widget;
}

int FreeComment::type() const
{
    return FreeComment::USERTYPE;
}

void FreeComment::setRect(const QRectF & rect)
{
   m_rect=rect;
}

void FreeComment::keyPressEvent(QKeyEvent *event)
{
    DiagramScene * myscene=static_cast<DiagramScene *>(this->scene());
    if (myscene->isPanelActive())
         return;
    if (isTextEditKey(event) &&
        myscene->editState()==TES_NONE)
    {
        ObjectTextEditor  * field=new ObjectTextEditor(myscene,this);

        const int border_hint=5;
        QRect tmp(m_rect.x(),m_rect.y(),m_rect.width(),m_rect.height()+2*border_hint);
        field->setGeometry(tmp);
        field->setFont(myscene->font());
        if (m_string!=DEFAULT_ALABEL_TEXT)
         field->setPlainText(m_string);
        //Move to end cursor
        QTextCursor c=field->textCursor();
        c.movePosition(QTextCursor::End);
        field->setTextCursor(c);
        QGraphicsProxyWidget * proxy=this->scene()->addWidget(field);
        proxy->update();
        myscene->toggleEditStateOn(field,proxy);
        field->grabKeyboard();
        field->keyPressEvent(event);
    }
    else
    {
        this->QGraphicsItem::keyPressEvent(event);
    }
}

void FreeComment::trySetText(const QString & text)
{
    DiagramScene * scene=static_cast<DiagramScene *>(this->scene());
    Diagram *       diag=scene->diagram();
    QFontMetrics metrics(scene->font());
    QRect        rect=metrics.boundingRect(0,0,100000,100000,Qt::AlignCenter,text);
    QRectF newrect(m_rect.x(),m_rect.y(),rect.width(),rect.height());
    if (diag->canBePlaced(newrect,this))
    {
        m_rect=newrect;
        m_string=text;
        scene->update();
    }
}
