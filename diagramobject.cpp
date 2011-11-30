#include "diagramobject.h"

void DiagramObject::setText(const QString & /*text*/)
{
    throw TextChangeNotImplemented();
}

void DiagramObject::paint(QPainter *painter,
                          const QStyleOptionGraphicsItem * /* option */,
                          QWidget * /* widget */)
{
  this->paint(painter);
}
