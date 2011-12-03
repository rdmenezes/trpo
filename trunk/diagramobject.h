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
#include "itemtypes.h"

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
        /*! Translate an object
         */
        inline QPointF translate( const QPointF & p ) const { return p-pos(); }
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
    /*! Whether we should draw roundings
     */
    virtual bool drawRoundings() const;
    /*! Returns a diagram for object
     */
    inline  Diagram * diagram() const
    {
        return m_diag;
    }
    /*! Clones a diagram object
     */
    virtual DiagramObject * clone()=0;
};

#endif // DIAGRAMOBJECT_H
