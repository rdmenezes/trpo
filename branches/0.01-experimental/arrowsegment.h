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

/*! Determines a direction of arrow
 */
ArrowDirection direction(const QPointF & in,const QPointF & out);

#define MIN_LENGTH_OF_SEGMENT 8
/*! Determines whether constructed segment will be too small
 */
bool tooSmall(ArrowPoint * in, const QPointF & out);


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
         /*! Currently does nothing
          */
         ~ArrowSegment();

};

#endif // ARROWSEGMENT_H
