/*! \file   alineitem.h
    \author Mamontov

    Describes an annotation line item
 */
#ifndef ALINEITEM_H
#define ALINEITEM_H

#include <QGraphicsItem>
#include <QPointF>
#include <QPainterPath>
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
        /*! Draws a cubic curve by points
            \param[in] p0   point
            \param[in] p1   point
            \param[in] p2   point
            \param[in] p3   point
            \param[in] t0   time, when p1 is reached
            \param[in] t1   time, when p2 is reached
            \param[out] path  path
         */
        void drawCubicCurve(const QPointF & p0,
                            const QPointF & p1,
                            const QPointF & p2,
                            const QPointF & p3,
                            double t0, double t1,
                            QPainterPath & path);
        /*! Draws a top type line item
            \param[out] path path
         */
        void drawTop(QPainterPath & path);
        /*! Draws a bottom type line item
            \param[out] path path
         */
        void drawBottom(QPainterPath & path);
        /*! Draws a left type line item
            \param[out] path path
         */
        void drawLeft(QPainterPath & path);
        /*! Draws a left type line item
            \param[out] path path
         */
        void drawRight(QPainterPath & path);
public:
        /*! Declares a type of item
         */
        enum ItemType
        {
            USERTYPE=QGraphicsItem::UserType+3
        };
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
         /*! Declares a painting event reaction
             \param[in] painter painting data
             \param[in] option
             \param[in] widget
          */
         void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
                        QWidget *widget);
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
         /*! Returns a type of segment
          */
         int type() const;
};

#endif // ALINEITEM_H
