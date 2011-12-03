/*! \file collisiondetector.h
    \author Mamontov
    Declares a collision detector class, that performs a collision testing
 */
#ifndef COLLISIONDETECTOR_H
#define COLLISIONDETECTOR_H
#include "collisionobject.h"
#include "collision.h"
#include <QHash>
#include <QPair>
/*! \class CollisionDetector
    Declares a collision detector
 */
class CollisionDetector
{
private:
        QHash< QPair<ShapeType,ShapeType>,
               bool (CollisionDetector::*) (CollisionObject *, CollisionObject *)
             > m_map; //!< Map data
        /*! Tests a collision between rectangles
            \param[in] o1 first object
            \param[in] o2 second object
            \return true if collides
         */
        bool testRectangles(CollisionObject * o1, CollisionObject *o2);
        /*! Tests a collision between rectangle and line
            \param[in] o1 first object
            \param[in] o2 second object
            \return true if collides
         */
        bool testRectangleAndLine(CollisionObject * o1, CollisionObject * o2);
        /*! Tests a collision between lines
            \param[in] o1 first object
            \param[in] o2 second object
            \return true if collides
         */
        bool testLines(CollisionObject * o1, CollisionObject * o2);
        /*! Tests a collision between complex object and other
            \param[in] o1 first object
            \param[in] o2 second object
            \return true if collides
         */
        bool testComplexAndOther(CollisionObject * o1,CollisionObject * o2);
public:
    /*! Constructs a collision detector
     */
    CollisionDetector();
    /*! Tests an object for collision
     */
    bool test(CollisionObject * o1, CollisionObject * o2);
};

#endif // COLLISIONDETECTOR_H
