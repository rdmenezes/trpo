#include "attachedcomment.h"
#include <assert.h>

void AttachedComment::paint(QPainter *p)
{
 //!< Do not implement, this class cannot be drawn
 assert("Do not draw attached comment!" && false);
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


