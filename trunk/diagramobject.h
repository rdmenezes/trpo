/*!  \file     diagramobject.h
     \author   MamontovDmitry
     Describes a diagram basic object ,which every item must subclass
*/
#ifndef DIAGRAMOBJECT_H
#define DIAGRAMOBJECT_H
#include <QGraphicsScene>
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
                     public CollisionObject,
                     public Serializable
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
    inline DiagramObject(ShapeType shape): QGraphicsItem(NULL),
                                           CollisionObject(shape)

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
    /*! Returns editor text
        \return text for editor
     */
    virtual const QString & getEditableText() const;
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
