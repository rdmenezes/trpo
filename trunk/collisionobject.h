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
  ST_COMPLEX
};

/*! Marker interface, that determines, whether object is collidable or not
 */
class CollisionObject
{
public:
       /*! Determines collision shape of item
           \return collision shape of item
        */
       virtual ShapeType collisionShape() const =0;
       /*! Virtual destructor
        */
       virtual ~CollisionObject();
};

/*! A rectangle-based collision object
 */
class  RectangularCollisionObject: public CollisionObject
{
 public:
    /*! Determines collision shape of item
        \return collision shape of itemû
     */
    ShapeType collisionShape() const;
    /*! Returns a collision rect for item
     */
    virtual const QRectF & collisionRect() const =0;
};
/*! A line-based collision object
 */
class  LinearCollisionObject: public CollisionObject
{
public:
    /*! Determines collision shape of item
        \return collision shape of itemû
     */
    ShapeType collisionShape() const;
    /*! Returns a collision rect for item
     */
    virtual const QLineF & collisionLine() const =0;
};
/*! A complex collision object, that consists from other objects
*/
class  ComplexCollisionObject: public CollisionObject
{
 public:
        /*! Determines collision shape of item
            \return collision shape of itemû
        */
       ShapeType collisionShape() const;
       /*! Returns first object for collision
       */
       virtual CollisionObject *    firstObject()=0;
       /*! Returns next object for collision
        */
       virtual CollisionObject *    nextObject()=0;
};

#endif // COLLISIONOBJECT_H
