/*! \file   alineitem.h
    \author Mamontov

    Describes an annotation line item
 */
#ifndef ALINEITEM_H
#define ALINEITEM_H

#include <QGraphicsItem>
#include <QPointF>
#include <QPainterPath>
#include "diagramscene.h"
#include "diagramobject.h"
#include "objectconnector.h"
#include <stdio.h>
#include "attachedcomment.h"
//A point of line segment
class ArrowPoint;
//A diagram data
class Diagram;
//Dom element
class QDomElement;
//Dom document
class QDomDocument;
//Loading data
class DiagramLoadData;


/*! Returns a collision point for objects
    \param[in] l line
    \param[i]  r rectangle
    \return   point
 */
inline QPointF * getCollisionPoint(const QLineF &  l, const QRectF & r,int ibeg=0)
{
    QLineF lines[4]={ QLineF(r.topLeft(),r.topRight()),
                      QLineF(r.topRight(),r.bottomRight()),
                      QLineF(r.bottomLeft(),r.bottomRight()),
                      QLineF(r.topLeft(),r.bottomLeft())
                    };
    QPointF result;
    for (int i=ibeg;i<4;i++)
    { 
        if (l.intersect(lines[i],&result)==QLineF::BoundedIntersection)
            return new QPointF(result);
    }
    return NULL;
}

/*! Returns a last collision point
    \param[in] l line
    \param[in] r rectangle
    \param[in] beg begin point
 */
QPointF getLastCollisionPoint(const QLineF & l ,const QRectF & r,const QPointF & beg);
class AttachedComment;
/*! \class CommentLine
    Declares an annotation line item
 */
class CommentLine : public DiagramObject
{
private:
        /*! Input connector data
         */
        ObjectConnector * m_self;
        /*! Parent comment data
         */
        AttachedComment * m_parentcomment;
        /*! Point from input connector
         */
        QPointF       m_in;
        /*! Point from output connector
         */
        QPointF       m_out;
        /*! Draws a cubic curve by points
            \param[in] p    pivot points
            \param[in] t0   time, when p1 is reached
            \param[in] t1   time, when p2 is reached
            \param[out] path  path
         */
        void drawCubicCurve(QPointF p[4],
                            double t0, double t1,
                            QPainterPath & path);
        /*! Draws an arcs
            \param[in]  path painter path
            \param[in]  l    length
            \param[in]  x    arc height
            \param[in]  hx   horizontal arc height
         */
        void drawArcs(QPainterPath & path,qreal l, qreal x,qreal hx);
protected:
        /*! Paints a line
            \param[in] p painter
         */
        void paint(QPainter * p);
        /*! Returns normal arc height
         */
        qreal getNormalArcHeight() const;
public:
        /*! Special constructor, used by serializable
         */
        inline CommentLine():DiagramObject(ST_LINE) {}
        /*! Creates a comment line
            \param[in] in  input
            \param[in] out output
            \param[in] oin input connector
            \param[in] out output connector
            \param[in] d   diagram
         */
        CommentLine(const QPointF & in,
                    const QPointF & out,
                    ObjectConnector * oin,
                    AttachedComment * oout,
                    Diagram * d );
        /*! Determines  a bounding rect
            \return bounding rectangle
         */
        QRectF boundingRect() const;
        /*!  Saves a data to document
             \param[in] doc     document data
             \param[in] element parent element data
         */
        virtual  void save(QDomDocument * doc,QDomElement * element);
        /*! Loads a default data from document populating address map
            \param[in] element element data
            \param[in] addressMap adressedMap
         */
        virtual  void load(QDomElement * element,
                           QMap<void *, Serializable *> & addressMap);
        /*! Resolves inner pointers, from stored in adress map
            \param[in] addressMap map of addresses
         */
        virtual  void resolvePointers(QMap<void *, Serializable *> & adressMap);
        /*! Returns a type of segment
         */
        int type() const;
        /*! Returns a collision rect for item
         */
        virtual QLineF  collisionLine() const;
        /*! Tests,whether a line contents a point
         */
        bool contains ( const QPointF & point ) const;
        /*! Clones a diagram object
         */
        virtual DiagramObject * clone();
        /*! Destructor
         */
        ~CommentLine();
        /*! Tests, whether has input
         */
        bool hasInput() const;
        /*! Sets an input for
            \param[in] c input connector
            \param[in] a  line position in connector
         */
        inline void setInput(ObjectConnector * c, qreal a)
        {
            m_self->addConnector(c,0,C_INPUT);
            c->addConnector(m_self,a,C_OUTPUT);
        }
        /*! Returns a binded input item
            \return NULL if not found
         */
        DiagramObject  * getInputObject();
        /*! Returns self connector
         */
        inline ObjectConnector * model()  { return m_self; }
        /*! Sets a line for item
            \param[in] x1 first pos
            \param[in] y1 first pos
            \param[in] x2 second pos
            \param[in] y2 second pos
         */
        void setLine(qreal x1,qreal y1,qreal x2,qreal y2);
        /*! Returns input point for a comment line
            \return input
         */
        inline const QPointF& in() const { return m_in;}
        /*! Returns output point for a comment line
            \return input
         */
        inline const QPointF& out() const { return m_out;}
         /*! Kills self if equal to object
             \param[in] o object to compare
             \return true if will die
          */
         virtual bool dieIfEqualTo(DiagramObject * o);

};

#endif // ALINEITEM_H
