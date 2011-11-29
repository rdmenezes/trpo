/*! \file    ArrowPoint
    \author  Mamontov

    Describes an arrow point, that contains  referenced data
 */
#ifndef ARROWPOINT_H
#define ARROWPOINT_H

#include <QPointF>
#include <QVector>
#include "moverange.h"
#include "arrowsegment.h"
//A diagram, where it's placed
class Diagram;
//An arrow segment, where data is placed
class ArrowSegment;
//A line ,where data is placed
class ALineItem;
//A class of box item
class Box;
//Dom element
class QDomElement;
//Dom document
class QDomDocument;
//Loading data
class DiagramLoadData;

enum ArrowDirection;

class ArrowPoint : public QPointF
{
private:
        /*! Describes a diagram data
        */
        Diagram * m_diag;
        /*! Determines, whether  arrow point is attached to an item
         */
        Box * m_block;
        /*!  Vector of segments, that output point is me
         */
        QVector<ArrowSegment *> m_in;
        /*!  Vector of segments, that input point is me
         */
        QVector<ArrowSegment *> m_out;
        /*!   Attached annotation lines
        */
        QVector<ALineItem * >  m_lines;
        /*! Generates moving restriction policy
            \param[out] result policy
            \param[in]  point  that determines restrictions
            \param[in]  dir    direction
            \param[in]  reversed reversed data
         */
        void generatePolicy(MoveRange & result,ArrowPoint * pivot,ArrowDirection dir,bool reversed);
public:
        /*! Determines, whether  arrow point is attached to an item
         */
        inline Box *& accessBlock() { return m_block; }
        /*!  Vector of segments, that output point is me
         */
        inline QVector<ArrowSegment *> & accessIn() { return m_in; }
        /*!  Vector of segments, that input point is me
         */
        inline QVector<ArrowSegment *> & accessOut() { return m_out; }
        /*!   Attached annotation lines
        */
        inline QVector<ALineItem * >  & accessLines() { return  m_lines; }
        /*! Detects, whether has annotations
         */
        bool hasAnnotations() { return m_lines.size()!=0; }
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
        /*! Determines, whether segment is incident to point
            \param[in] p point
         */
        bool isIncident(ArrowSegment * s);
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
        bool tryRemoveSegment(ArrowSegment * segment,bool correct_arcs=true);
        /*! Removes an annotation from segment
            \param[in] line line, that will be removed
         */
        void removeAnnotation(ALineItem * line);
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
        inline Box * attachedBlock() { return m_block; }
        /*! Attaches a block to a point
            \param[in] block block to be attached
         */
        inline void attachBlock(Box * b) {m_block=b;}
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
        /*! Returns a moving range for a point
            \param[in] exc - excluded segment (NULL to handle all)
         */
        MoveRange moveRange(ArrowSegment * exc=NULL);
        /*! Determines, whether point can move to
            \param[in] pos new position
            \param[in] exc excluded segment (NULL to handle all)
         */
        bool canMoveTo(const QPointF & pos, ArrowSegment * exc=NULL);
        /*! Returns a lines.
         */
        inline QVector<ALineItem * > & annotationLines() { return m_lines; }
        /*! Saves a point
            \param[in] doc document
            \param[in] diagram diagram
         */
        void save(QDomDocument * doc,QDomElement * diagram);
};

#endif // ARROWPOINT_H
