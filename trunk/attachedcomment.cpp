#include "attachedcomment.h"
#include <assert.h>
#include <QMessageBox>
#include "freecomment.h"

void AttachedComment::paint(QPainter *p)
{
  if (m_comment)
  {
        p->translate(m_comment->pos());
        m_comment->DiagramObject::paint(p,NULL,NULL);
        p->resetTransform();
  }
  if (m_line)
  {
      p->translate(m_line->pos());
      m_line->DiagramObject::paint(p,NULL,NULL);
      p->resetTransform();
  }
}

QRectF AttachedComment::boundingRect() const
{
    return QRectF();
}

void AttachedComment::save(QDomDocument * doc,
               QDomElement *  element)
{
    QDomElement ac;
    ac=doc->createElement("attachedcomment");
    pushThis(ac);
    ac.setAttribute("pos", ::save(this->pos()));
    ac.setAttribute("diagram",::save(m_diag));
    ac.setAttribute("fc",::save(m_comment));
    ac.setAttribute("line",::save(m_line));
    ac.setAttribute("iterator",::save(m_iterator));


    if (m_comment) m_comment->save(doc,&ac);
    if (m_line) m_line->save(doc,&ac);

    element->appendChild(ac);
}

void AttachedComment::load(QDomElement *  element ,
               QMap<void *, Serializable *> &  addressMap  )
{
    Serializable *p=NULL;
    QDomNamedNodeMap attributes=element->attributes();
    qload(attributes,"this",p);
    addressMap.insert(p,this);
    QPointF pos;
    qload(attributes,"pos",pos);
    setPos(pos);
    qload(attributes,"diagram",m_diag);
    qload(attributes,"fc",m_comment);
    qload(attributes,"line",m_line);
    qload(attributes,"iterator",m_iterator);
    QDomNode diagram=element->firstChild();
    while(! (diagram.isNull()))
    {
        if (diagram.isElement())
        {
            QDomElement el=diagram.toElement();
            if (el.tagName()=="fc")
            {
                FreeComment * fc=new FreeComment();
                fc->load(&el,addressMap);
            }
            if (el.tagName()=="line")
            {
                CommentLine * fc=new CommentLine();
                fc->load(&el,addressMap);
            }
        }
        diagram=diagram.nextSibling();
    }
}

void AttachedComment::resolvePointers(QMap<void *, Serializable *> &
                           adressMap )
{
    m_diag=static_cast<Diagram*>(adressMap[m_diag]);
    m_comment=static_cast<FreeComment*>(adressMap[m_comment]);
    m_comment->resolvePointers(adressMap);
    if (m_line)
    {
        m_line=static_cast<CommentLine*>(adressMap[m_line]);
        m_line->resolvePointers(adressMap);
    }
}

CollisionObject *    AttachedComment::firstObject()
{
  m_iterator=0;
  return comment();
}

CollisionObject *    AttachedComment::nextObject()
{
    if (m_iterator==0)
    {
        m_iterator=1;
        return line();
    }
    return NULL;
}

DiagramObject * AttachedComment::clone()
{
 return NULL;
}

bool AttachedComment::deleteOnRemoval()
{
  if (m_line)
  {
      m_line->scene()->removeItem(m_line);
      delete m_line;
  }
  if (m_comment)
  {
      m_comment->scene()->removeItem(m_comment);
      delete m_comment;
  }
  return false;
}

bool AttachedComment::dieIfEqualTo(DiagramObject * o)
{
  if (m_comment->dieIfEqualTo(o))
  {
      if (m_line)
      {
          CommentLine * line=m_line;
          line->dieIfEqualTo(line);
          delete line;
      }
      delete m_comment;
      return true;
  }
  else
  {
    if (m_line)
    {
      CommentLine * line=m_line;
      if (m_line->dieIfEqualTo(o))
      {
          delete line;
          m_line=NULL;
      }
    }
    return false;
  }
}

void AttachedComment::addToScene(QGraphicsScene * scene)
{
    if (m_comment)
        m_comment->addToScene(scene);
    if (m_line)
        m_line->addToScene(scene);
}
