/*! \file   alineitem.h
    \author Mamontov

    Describes an annotation line item
 */
#ifndef ALINEITEM_H
#define ALINEITEM_H

#include <QGraphicsItem>
#include <QPointF>

//A point of line segment
class ArrowPoint;
//A diagram data
class Diagram;

/*! \class ALineItem
    Declares an annotation line item
 */
class ALineItem : public QGraphicsItem
{
private:
        /*! A free point, that is pointed into air
         */
        QPointF m_freepoint;
        /*! A more complicated, binded point to line, or pushed into air
            (if attached into block)
         */
        QPointF * m_bindedpoint;
        /*! Declares, whether an item binded into line
         */
        bool      m_isbindedtoline;
        /*! Diagram, where item is placed;
         */
        Diagram * m_diagram;
        /*! Casts a point into binded point
         */
        ArrowPoint * binded();
public:
         inline QPointF * begin() { return m_bindedpoint;}
         inline QPointF * end()   { return &m_freepoint; }
         /*! Constructs an annotation line, not binded into any of points
            \param[in] bindedpoint free binded point (first)
            \param[in] freepoint   free point (second)
          */
         ALineItem(const QPointF & bindedpoint,const QPointF & freepoint);
         /*! Constructs an annotation line, binded into a line
            \param[in] bindedpoint  non-free binded point (first)
            \param[in] freepoint    free point (second)
          */
         ALineItem(QPointF * bindedpoint,const QPointF & freepoint);
         /*! Sets a diagram for annotation line
            \param[in] diag diagram
          */
         inline  void setDiagram(Diagram * diag) { m_diagram=diag; }
         /*! Determines  a bounding rect
          */
         QRectF boundingRect() const;
         /*! Deattaches an item from line
          */
         void deattachFromLine();
         /*! Is attached to line
          */
         inline bool isAttachedToLine() const { return m_isbindedtoline; }
         /*! Kills a line item, removing it from any of places
          */
         void die();
         /*! Destructor
          */
         ~ALineItem();
};

#endif // ALINEITEM_H
