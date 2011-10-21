/*! \file    ArrowPoint
    \author  Mamontov

    Describes an arrow point, that contains  referenced data
 */
#ifndef ARROWPOINT_H
#define ARROWPOINT_H

#include <QPointF>
#include <QVector>

//A diagram, where it's placed
class Diagram;
//An arrow segment, where data is placed
class ArrowSegment;
//A line ,where data is placed
class ALineItem;
//A class of box item
class BoxItem;

enum ArrowDirection;

class ArrowPoint : public QPointF
{
private:
        /*! Describes a diagram data
        */
        Diagram * m_diag;
        /*! Determines, whether  arrow point is attached to an item
         */
        BoxItem * m_block;
        /*!  Vector of segments, that output point is me
         */
        QVector<ArrowSegment *> m_in;
        /*!  Vector of segments, that input point is me
         */
        QVector<ArrowSegment *> m_out;
        /*!   Attached annotation lines
        */
        QVector<ALineItem * >  m_lines;
public:
        /*! Constructs a new arrow point
            \param[in] x x point
            \param[in] y y point
         */
        ArrowPoint(qreal x, qreal y);
        /*! Returns an input segments
         */
        inline const QVector<ArrowSegment *> & inputSegments() { return m_in; }
        /*! Returns an output segments
         */
        inline const QVector<ArrowSegment *> & outputSegments() { return m_out; }
        /*! Has a segment with direction
            \param[in] d direction
         */
        bool hasInputSegment(ArrowDirection * d);
        /*! Tests, whether it has the same directed segment
         */
        bool hasSameDirectedSegment(QPointF * in, QPointF * out);
        /*! Has a segment with direction
            \param[in] d direction
         */
        bool hasOutputSegment(ArrowDirection * d);
        /*! Detects, whether has one input segment with exact direction
         */
        bool hasSameInputSegment(QPointF * in, QPointF * out);
        /*! Detects, whether constructed segment has opposite direction with other
         */
        bool hasOppositeSegment(QPointF * in, QPointF * out);
        /*! Adds a new segment, which output point is me
            \param[in] segment new segment
         */
        void addInputSegment(ArrowSegment * segment);
        /*! Adds a new segment, which input point is me
            \param[in] segment new segment
         */
        void addOutputSegment(ArrowSegment * segment);
        /*! Adds an annotation line, which is attached line
            \param[in] line annotation line
         */
        void attachAnnotation(ALineItem * line);
        /*! Tries a segment removal
            \param[in] segment segment data
            \return true, on success false, if can't be
         */
        bool tryRemoveSegment(ArrowSegment * segment);
        /*! Removes an annotation from segment
            \param[in] line line, that will be removed
         */
        bool removeAnnotation(ALineItem * line);
        /*! Removes a point from diagram
         */
        void die();
        /*! Tests, whether point can be removed, if two lines has same direction
            and it's only one
         */
        bool canBeRemoved();
        /*! Returns attached block
            \param[in] block block data
         */
        inline BoxItem * attachedBlock() { return m_block; }
        /*! Attaches a block to a point
            \param[in] block block to be attached
         */
        inline void attachBlock(BoxItem * b) {m_block=b;}
        /*! Deattaches a block from a point
         */
        inline void deattachFromBlock() { m_block=NULL; }
        /*! Detects, whether point is starting point for a segments
         */
        bool isBeginPoint() const;
        /*! Detects, whether point is ending point for a segments
         */
        bool isEndingPoint() const;
        /*! Detects, whether point is invalid
         */
        bool mustBeDecomposed() const;
        /*! Detects, whether point is separated
         */
        bool isSeparated() const;
        /*! Updates a point incident segments
         */
        void update();
        /*! Sets a diagram
            \param[in] p diagram
         */
        inline void setDiagram(Diagram * p) { m_diag=p;}
        /*! Destructor
         */
        ~ArrowPoint();
};

#endif // ARROWPOINT_H
