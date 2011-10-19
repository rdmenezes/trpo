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
         /*! Currently does nothing
          */
         ~ArrowSegment();
};

#endif // ARROWSEGMENT_H
