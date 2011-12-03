/*!  \file     collisionobject.h
     \author   MamontovDmitry
     Describes an object, which can be collidable or not
*/
#ifndef COLLISIONOBJECT_H
#define COLLISIONOBJECT_H

#include <QRectF>
#include <QLineF>

/*! Declares a shape type of item
*/
enum  ShapeType
{
  ST_RECTANGLE,
  ST_LINE,
  ST_COMPLEX,
  ST_NONE
};

/*! Exception, which is thrown if attempted to work with
    object of one collision type, like another
 */
class CollisionDataNotSpecified {
public:
      inline CollisionDataNotSpecified(){}
};
/*! Marker interface, that determines, whether object is collidable or not
 */
class CollisionObject
{
private:
        ShapeType  m_shapetype;
public:
       inline CollisionObject(ShapeType type) { m_shapetype=type; }
       /*! Determines collision shape of item
           \return collision shape of item
        */
       inline ShapeType collisionShape() const { return m_shapetype; }
       /*! Returns a collision rect for item
        */
       virtual QRectF  collisionRect() const;
       /*! Returns a collision rect for item
        */
       virtual QLineF  collisionLine() const ;
       /*! Returns first object for collision
       */
       virtual CollisionObject *    firstObject();
       /*! Returns next object for collision
        */
       virtual CollisionObject *    nextObject();
       /*! Virtual destructor
        */
       virtual ~CollisionObject();
};

#endif // COLLISIONOBJECT_H
