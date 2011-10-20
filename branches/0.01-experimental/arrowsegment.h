/*! \file   arrowsegment.h
    \author Mamontov

    Describes an arrow segment, that describes an arrow, connecting two points
 */
#ifndef ARROWSEGMENT_H
#define ARROWSEGMENT_H

#include <QGraphicsItem>

//A point of line segment
class ArrowPoint;
//A diagram data
class Diagram;



enum ArrowDirection
{
    AD_LEFT,
    AD_RIGHT,
    AD_TOP,
    AD_BOTTOM,
};

QPointF  computeOutputPoint(const QPointF & pivot,ArrowDirection & dir);
QPointF  computeInputPoint(const QPointF & pivot,ArrowDirection & dir);
/*! Determines a direction of arrow
 */
ArrowDirection direction(const QPointF & in,const QPointF & out);

#define MIN_LENGTH_OF_SEGMENT 8
/*! Determines whether constructed segment will be too small
 */
bool tooSmall(ArrowPoint * in, const QPointF & out);

/*! Constructs a second point ofdirected line, that can be used to build a segment
 */
QPointF constructDirectedLine(const QPointF & point1,const QPointF & point2);
/*! \class ArrowSegment
    Describes an arrow segment, that connects two lines
 */
class ArrowSegment : public QGraphicsItem
{
private:
         /*!  Input point
          */
         ArrowPoint * m_in;
         /*!  Output point
          */
         ArrowPoint * m_out;
         /*!  Diagram data
          */
         Diagram    * m_diag;
         /*! Draws output arc if self direction is left
          */
         void drawOALeft(const QPointF & pivot, ArrowSegment * seg,QPainter * painter);
         /*! Draws output arc if self direction is right
          */
         void drawOARight(const QPointF & pivot, ArrowSegment * seg,QPainter * painter);
         /*! Draws output arc if self direction is top
          */
         void drawOATop(const QPointF & pivot, ArrowSegment * seg,QPainter * painter);
         /*! Draws output arc if self direction is bottom
          */
         void drawOABottom(const QPointF & pivot, ArrowSegment * seg,QPainter * painter);
         /*! Draws output arc to a segment
             \param[in] pivot output point
             \param[in] mydir direction
             \param[in] seg segment
             \param[in] painter painter
          */
         void drawOutputArc(const QPointF & pivot, ArrowDirection mydir,
                            ArrowSegment * seg,QPainter * painter);
public:
         /*! Returns an input point
          */
         inline ArrowPoint * in() { return m_in; }
         /*! Returns an output point
          */
         inline ArrowPoint * out() { return m_out; }
         /*! Describes a segment
             \param[in] in   input  point
             \param[in] out  output point
          */
         ArrowSegment(ArrowPoint * in, ArrowPoint * out);
         /*! Removes an object from every place, where it's referenced
          */
         /*! Sets a diagram data for a segment
             \param[in] diag diagram
         */
         inline void setDiagram(Diagram * diag) { m_diag=diag; }
         /*! Determines self direction
          */
         ArrowDirection direction();
         /*! Determines  a bounding rect
          */
         QRectF boundingRect();
         /*! Declares a painting event data
             \param[in] painter painting data
             \param[in] option
             \param[in] widget
          */
         void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
                        QWidget *widget);
         /*! Currently does nothing
          */
         ~ArrowSegment();

};

#endif // ARROWSEGMENT_H
