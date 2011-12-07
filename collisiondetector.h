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
#include <QVector>
/*! \class CollisionDetector
    Declares a collision detector
 */
class CollisionDetector
{
private:
        QHash< QPair<ShapeType,ShapeType>,
               bool (CollisionDetector::*) (CollisionObject *, CollisionObject *)
             > m_map; //!< Map data

        QVector<int> m_exctypes;                   //!< Excluded types from scan
        QVector<CollisionObject *> m_objects;      //!< Excluded collision objects
        CollisionObject        *   m_tested_object;  //!< Object, used for testing
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
        \param[in] exctypes excluded types
        \param[in] objects  excluded objects
        \param[in] object   tested object
     */
    CollisionDetector(const QVector<int> & exctypes,
                      const QVector<CollisionObject *> & objects,
                      CollisionObject * object);

    /*! Tests an object for collision
     */
    bool test(CollisionObject * o1, CollisionObject * o2);
};

#endif // COLLISIONDETECTOR_H
