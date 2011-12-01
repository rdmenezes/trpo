/*! \file objectconnector.h
    \author HiddenSeeker

    Declares an object connector type
 */
#include "connection.h"
#include "direction.h"
#include "diagramobject.h"
#include <QLineF>
#include <QVector>
#include <QPair>
#ifndef OBJECTCONNECTOR_H
#define OBJECTCONNECTOR_H

/*! \class ObjectConnector
    Class, which creates a connectors for data
 */
class ObjectConnector: public QLineF
{
 private:
         DiagramObject        *   m_parent;
         /*! Relative positions of connected objects
          */
         QVector< QPair<qreal,ObjectConnector*> > m_connected[2];
         /*! Directions of objects, which are acceptable
          */
         QVector<Direction>       m_acceptable_directions;
         /*! Acceptable connection types
          */
         QVector<Connection>      m_acceptable_connections;
         /*! Acceptable types
          */
         QVector<int>             m_acceptable_types;
 protected:
        /*! Broadcasts move signal for each positions of units
            \return true on success
         */
        bool broadcastMoveSignal();
 public:
        /*! Empty connector
         */
        inline ObjectConnector(): QLineF() {}
        /*! Constructor from line
            \param[in] l line
         */
        inline ObjectConnector(const QLineF & l): QLineF(l) { }
        /*! Constructs a point from two lines
            \param[in] x1 first point X coordinate
            \param[in] y1 first point Y coordinate
            \param[in] x2 second point X coordinate
            \param[in] y2 second point Y coordinate
         */
        inline ObjectConnector(qreal x1, qreal y1, qreal x2, qreal y2)
            : QLineF(x1,y1,x2,y2)
        {}
        /*! Sets a parent object for connector
            \param[in] parent parent object
         */
        inline void setParent(DiagramObject * parent) { m_parent=parent;}
        /*! Resizes a line without saving absolute coordinates
            \param[in] l   new line
         */
        bool resize(const QLineF & l);
        /*! Moves a line, trying to move all objects
            \param[in] l new pos
         */
        bool tryMove(const QPointF & l);
        /*! Resizes a line, saving absolute object coordinates
            \param[in] l  new line
         */
        void enlarge(const QLineF & l);
        /*! Returns a direction of connector
            \return direction
         */
        Direction direction() const;
        /*! Merges one connector with another
            \param[in] o other connector
         */
        void merge(ObjectConnector * o);
        /*! Tests, whether we should draw roundings for items
         */
        inline bool drawRoundings() const { return m_parent->drawRoundings(); }

        /*! Adds a new connector
            \param[in] c     connector
            \param[in] point adding point
         */
        bool addConnector(ObjectConnector * c, qreal point,Connection type);
        /*! Returns a connected objects
            \param[in] point point
            \return    new   object
         */
        QVector<ObjectConnector *>  getConnected(qreal point,Connection type);
};


/*! \class ObjectConnectorFactory
    Class, that creates a  specific connectors for objects
 */
class ObjectConnectorFactory
{
 private:
         /*! Creates a connectors for a rectangles
             \param[in] r rectangle
             \return vector of connectors
          */
         static QVector<ObjectConnector *> createForRect(const QRectF & r);
 public:
         /*! Creates a connectors for a rectangles
             \param[in] r rectangle
             \return vector of connectors
          */
         static QVector<ObjectConnector *> createForBox(const QRectF & r);
         /*! Creates a connectors for a rectangles
             \param[in] r rectangle
             \return vector of connectors
          */
         static QVector<ObjectConnector *> createForComment(const QRectF & r);
};

/*! Free all connectors
    \param[in] cts connectors
 */
inline void freeConnectors( const QVector<ObjectConnector*> & cts)
{
   for (int i=0;i<cts.size();i++)
       delete cts[i];
}


#endif // OBJECTCONNECTOR_H
