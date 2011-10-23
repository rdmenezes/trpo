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
//Dom element
class QDomElement;
//Dom document
class QDomDocument;


enum ArrowDirection
{
    AD_LEFT,
    AD_RIGHT,
    AD_TOP,
    AD_BOTTOM,
};

QPointF  computeOutputPoint(const QPointF & pivot,ArrowDirection & dir);
QPointF  computeInputPoint(const QPointF & pivot,ArrowDirection & dir);

/*! Determines, whether two sequential segments can be merged
    \param[in] last_point first point of adding segment
    \param[in] endpoint  beginning of seconde segment
    \param[in] segdir    second segment direction
    \param[in] mydir     own segment direction
    \param[in] test      point ,where it must be moved
 */
bool canBeMerged(ArrowPoint * last_point,const QPointF & endpoint,
                 ArrowDirection segdir, ArrowDirection mydir,QPointF & test );


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
         /*! Declares a type of item
          */
         enum ItemType
         {
             USERTYPE=QGraphicsItem::UserType+2
         };
         //Does nothing
         inline ArrowSegment() {}
         /*! Returns an input point
          */
         inline ArrowPoint *& accessIn() { return m_in; }
         /*! Returns an output point
          */
         inline ArrowPoint *& accessOut() { return m_out; }
         /*! Returns an input point
          */
         inline ArrowPoint * in() { return m_in; }
         /*! Returns an output point
          */
         inline ArrowPoint * out() { return m_out; }
         /*! Sets an input point
          */
         inline void setIn(ArrowPoint * p) {m_in=p;}
         /*! Sets an output point
          */
         inline void setOut(ArrowPoint * p) {m_out=p;}
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
         ArrowDirection direction() const;
         /*! Determines  a bounding rect
          */
         QRectF boundingRect() const;
         /*! Declares a painting event reaction
             \param[in] painter painting data
             \param[in] option
             \param[in] widget
          */
         void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
                        QWidget *widget);
         /*! Currently does nothing
          */
         ~ArrowSegment();
         /*! Returns a type of segment
          */
         int type() const;
         /*! Kills a segment
          */
         void die();
         /*! Saves a segment
             \param[in] doc document
             \param[in] diagram diagram
          */
         void save(QDomDocument * doc,QDomElement * diagram);

};

#endif // ARROWSEGMENT_H
