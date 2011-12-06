#include "attachedcomment.h"
#include <assert.h>

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

void AttachedComment::save(QDomDocument * /* doc */,
               QDomElement *  /* element */)
{
    //!< TODO: Implement this later
}

void AttachedComment::load(QDomElement * /* element */,
               QMap<void *, Serializable *> & /* addressMap */ )
{
    //!< TODO: Implement this later
}

void AttachedComment::resolvePointers(QMap<void *, Serializable *> &
                          /* adressMap */)
{
    //!< TODO: Implement this later
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
  if (m_comment)
      m_comment->scene()->removeItem(m_comment);
  if (m_line)
      m_line->scene()->removeItem(m_line);
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
      CommentLine * line=m_line;
      if (m_line->dieIfEqualTo(o))
      {
          delete line;
          m_line=NULL;
      }
      return false;
  }
}
