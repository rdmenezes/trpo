#include "diagramobject.h"

void DiagramObject::setText(const QString & /*text*/)
{
    throw TextChangeNotImplemented();
}

const QString & DiagramObject::getEditableText() const
{
  throw TextChangeNotImplemented();
  return *(new QString());
}

void DiagramObject::paint(QPainter *painter,
                          const QStyleOptionGraphicsItem * /* option */,
                          QWidget * /* widget */)
{
  this->paint(painter);
}


bool DiagramObject::drawRoundings() const
{
    return false;
}

bool DiagramObject::deleteOnRemoval()
{
  return true;
}

int DiagramObject::collisionObjectType() const
{
    return type();
}
