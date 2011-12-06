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
        /*! Returns a parent object for connector
            \return parent object
         */
        inline DiagramObject * parent() { return m_parent; }
        /*! Resizes a line without saving absolute coordinates
            \param[in] l   new line
         */
        bool resize(const QLineF & l);
        /*! Moves a line, trying to move all objects
            \param[in] l new pos
         */
        bool tryMove(const QPointF & l);
        /*! Resizes a line, saving absolute object coordinates
            \param[in] newend  new ending position
            \param[in] end     end data
         */
        void enlarge(const QPointF & newend,bool end=true);
        /*! Returns a direction of connector
            \return direction
         */
        Direction direction() const;
        /*! Tests, whether we should draw roundings for items
         */
        inline bool drawRoundings() const
        {
            if (!m_parent)
                return false;
            else
                return m_parent->drawRoundings();
        }
        /*! Tests,whether we should draw triangle
         */
        inline bool drawTriangle() const
        {
            if (!m_parent)
                return true;
            else
                return m_parent->drawTrianglePart();
        }
        /*! Adds a new connector
            \param[in] c     connector
            \param[in] point adding point
            \param[in] type  type data
            \param[in] debug debug data
         */
        bool addConnector(ObjectConnector * c, qreal point,Connection type,bool debug=true);
        /*! Returns a connected objects
            \param[in] point point
            \return    new   object
         */
        QVector<ObjectConnector *>  getConnected(qreal point,Connection type);
        /*!  Returns a connected objects
             \param[in] type connection type
             \return new type
         */
        inline const QVector< QPair<qreal,ObjectConnector*> > & getConnected(Connection type) const
        { return m_connected[type]; }
        /*! Determines,whether connector has connected another connector
            \param[in] o other connector
         */
        bool hasConnected(ObjectConnector * o);
        /*! Removes a connector from list
            \param[in] o other connector
         */
        void remove(ObjectConnector * o);
        /*! Clears all references other connectors
         */
        void clear();
};



/*! Free all connectors
    \param[in] cts connectors
 */
inline void freeConnectors( const QVector<ObjectConnector*> & cts)
{
   for (int i=0;i<cts.size();i++)
       delete cts[i];
}

/*! Tests, whether from start to goal exists some way
    \param[in] start starting point
    \param[in] goal  ending point
 */
bool hasWay(ObjectConnector * start, ObjectConnector * goal);

#endif // OBJECTCONNECTOR_H
