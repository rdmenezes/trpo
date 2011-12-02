#include "freecomment.h"
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
    QRectF rect=computeRect(text);
    setX(rect.x());
    setY(rect.y());
    m_size=rect.size();
    m_text=text;
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
    return FreeComment::USERTYPE;
}

void FreeComment::setRect(const QRectF & rect)
{
   //m_rect=rect;
}

void FreeComment::keyPressEvent(QKeyEvent *event)
{
    DiagramScene * myscene=static_cast<DiagramScene *>(this->scene());
    if (myscene->isPanelActive())
         return;
    if (isTextEditKey(event) &&
        myscene->editState()==TES_NONE)
    {
        ObjectTextEditor  * field=new ObjectTextEditor(myscene,NULL);

        const int border_hint=5;
        QRect tmp;//(m_rect.x(),m_rect.y(),m_rect.width(),m_rect.height()+2*border_hint);
        field->setGeometry(tmp);
        field->setFont(myscene->font());
        //if (m_string!=DEFAULT_ALABEL_TEXT)
        // field->setPlainText(m_string);
        //Move to end cursor
        QTextCursor c=field->textCursor();
        c.movePosition(QTextCursor::End);
        field->setTextCursor(c);
        QGraphicsProxyWidget * proxy=this->scene()->addWidget(field);
        proxy->update();
        //myscene->toggleEditStateOn(field,proxy);
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
    QRectF newrect;//(m_rect.x(),m_rect.y(),rect.width(),rect.height());
    if (diag->canBePlaced(newrect,this))
    {
        //m_rect=;
        m_text=text;
        scene->update();
    }
}
