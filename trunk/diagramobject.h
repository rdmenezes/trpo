/*!  \file     diagramobject.h
     \author   MamontovDmitry
     Describes a diagram basic object ,which every item must subclass
*/
#ifndef DIAGRAMOBJECT_H
#define DIAGRAMOBJECT_H
#include <QGraphicsItem>
#include "collisionobject.h"
#include "serializable.h"

/*! \class DiagramObject
    A diagram object is a graphic item, which can be serialized
    an can have collision
 */
class DiagramObject: public QGraphicsItem,
                            CollisionObject,
                            Serializable
{
public:
    /*! Creates a new object
        \param[in] shape shape data
     */
    inline DiagramObject(ShapeType shape): CollisionObject(shape),
                                           QGraphicsItem(NULL)
    {

    }
};

#endif // DIAGRAMOBJECT_H
