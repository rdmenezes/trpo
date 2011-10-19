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
        /*! Attaches a block to a point
            \param[in] block block to be attached
         */
        inline void attachBlock(BoxItem * b) {m_block=b;}
        /*! Deattaches a block from a point
         */
        inline void deattachFromBlock() { m_block=NULL; }
        /*! Detects, whether point is starting point for a segments
         */
        bool isBeginPoint();
        /*! Detects, whether point is ending point for a segments
         */
        bool isEndingPoint();
        /*! Detects, whether point is invalid
         */
        bool mustBeDecomposed();
        /*! Detects, whether point is separated
         */
        bool isSeparated();
        /*! Destructor
         */
        ~ArrowPoint();
};

#endif // ARROWPOINT_H
