#include "diagramobject.h"
#include "commentline.h"

void DiagramObject::setText(const QString & /*text*/)
{
    throw TextChangeNotImplemented();
}

QString  DiagramObject::getEditableText() const
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

bool DiagramObject::drawTrianglePart() const
{
    return false;
}

int DiagramObject::collisionObjectType() const
{
    return type();
}

QPointF DiagramObject::receiveCommentLineMove(DiagramObject * obj)
{
  return static_cast<CommentLine*>(obj)->in();
}

void DiagramObject::addToScene(QGraphicsScene * scene)
{
    scene->addItem(this);
}
