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


class AttachedComment;
/*! \class CommentLine
    Declares an annotation line item
 */
class CommentLine : public DiagramObject
{
private:
        /*! Input connector data
         */
        ObjectConnector * m_input;
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
        /*! Draws an arcs
            \param[in]  path painter path
            \param[in]  l    length
            \param[in]  x    arc height
            \param[in]  hx   horizontal arc height
         */
        void drawArcs(QPainterPath & path,qreal l, qreal x,qreal hx);


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



        /*! A free point, that is pointed into air
         */
        inline QPointF & accessFree() { return m_freepoint; }
        /*! A more complicated, binded point to line, or pushed into air
            (if attached into block)
         */
        inline QPointF *& accessBinded() { return m_bindedpoint; }
        /*! Declares, whether an item binded into line
         */
        inline bool  &   accessIsBinded() { return m_isbindedtoline; }
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
         CommentLine(const QPointF & bindedpoint,const QPointF & freepoint);
         /*! Constructs an annotation line, binded into a line
            \param[in] bindedpoint  non-free binded point (first)
            \param[in] freepoint    free point (second)
          */
         CommentLine(QPointF * bindedpoint,const QPointF & freepoint);
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
         ~CommentLine();
         /*! Returns a type of segment
          */
         int type() const;
};

#endif // ALINEITEM_H
