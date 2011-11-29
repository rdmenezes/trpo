/*! \file objectconnector.h
    \author HiddenSeeker

    Declares an object connector type
 */
#include "connection.h"
#include "direction.h"
#include "diagramobject.h"
#include <QLineF>
#include <QVector>
#ifndef OBJECTCONNECTOR_H
#define OBJECTCONNECTOR_H

/*! \class ObjectConnector
    Class, which creates a connectors for data
 */
class ObjectConnector
{
 private:
         /*! Relative positions of connected objects
          */
         QVector<qreal>           m_connected_positions;
         /*! Binded objects
          */
         QVector<DiagramObject*>  m_connected_objects;
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
