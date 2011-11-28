/*!  \file     diagramobject.h
     \author   MamontovDmitry
     Describes a diagram basic object ,which every item must subclass
*/
#ifndef DIAGRAMOBJECT_H
#define DIAGRAMOBJECT_H
#include <QGraphicsItem>
#include <QStyleOptionGraphicsItem>
#include <QWidget>
#include "collisionobject.h"
#include "serializable.h"

class Diagram;

class TextChangeNotImplemented
{
 public:
      inline   TextChangeNotImplemented() {}
};

/*! \class DiagramObject
    A diagram object is a graphic item, which can be serialized
    an can have collision
 */
class DiagramObject: public QGraphicsItem,
                            CollisionObject,
                            Serializable
{
protected:
        Diagram * m_diag;  //!< Diagram
        /*! Paints an object (real)
            \param[in] p paint
         */
        virtual void paint(QPainter * p)=0;
public:
    /*! Creates a new object
        \param[in] shape shape data
     */
    inline DiagramObject(ShapeType shape): CollisionObject(shape),
                                           QGraphicsItem(NULL)
    {

    }
    /*! Set diagram
        \param[in] d diagram
     */
    inline void setDiagram(Diagram * d) { m_diag=d;}

    /*! Set text
        \param[in] text text data
     */
    virtual void setText(const QString & text);
    /*! Paint event reaction
        \param[in] painter painter drawing
        \param[in] option options
        \param[in] widget widget data
     */
    void paint(QPainter *painter,
               const QStyleOptionGraphicsItem *option,
               QWidget *widget);
};

#endif // DIAGRAMOBJECT_H
