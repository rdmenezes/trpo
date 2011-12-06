#include "diagramobjecttest.h"
#include "itemtypes.h"

QRectF DiagramObjectTest::boundingRect() const
{
    return QRectF(0,0,m_img.width(),m_img.height());
}

void DiagramObjectTest::paint(QPainter *p)
{
  p->drawImage(boundingRect(),m_img);
}

int DiagramObjectTest::type() const
{
    return IsDefaultItem;
}

void DiagramObjectTest::save(QDomDocument * /* doc */,
               QDomElement *  /* element */)
{
    //!< TODO: Implement this later
}

void DiagramObjectTest::load(QDomElement * /* element */,
               QMap<void *, Serializable *> & /* addressMap */ )
{
    //!< TODO: Implement this later
}

void DiagramObjectTest::resolvePointers(QMap<void *, Serializable *> &
                          /* adressMap */)
{
    //!< TODO: Implement this later
}

DiagramObject * DiagramObjectTest::clone()
{
    return new DiagramObjectTest(this->pos(),this->diagram());
}

bool DiagramObjectTest::dieIfEqualTo(DiagramObject * o)
{
    return false;
}
