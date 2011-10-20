/*! \class  collision.h
    \author HiddenSeeker
    Declares a collision testing functions
 */
#include <QRectF>
#include <QRect>
#include <QPoint>
#include <QPointF>

#ifndef COLLISION_H
#define COLLISION_H

class ArrowPoint;
/*! Tests a collision between two rectangles
    \param[in] r1 first rectangle
    \param[in] r2 second rectangle
 */
bool collides(const QRectF & r1,const QRectF & r2);
/*! Tests a collision between two rectangles
    \param[in] r1 first rectangle
    \param[in] r2 second rectangle
 */
bool collides(const QRect & r1,const QRect & r2);
/*! Tests a collision between two rectangles
    \param[in] r1 first rectangle
    \param[in] p1 first point
    \param[in] p2 second point
 */
bool collides(const QRectF & r,const QPoint & p1, const QPoint & p2);
/*! Tests, whether lines are colliding by bounds
 */
bool boundaryCollides(const QRectF & r, const ArrowPoint * p1, const ArrowPoint * p2);
/*! Tests a collision between two rectangles
    \param[in] r1 first rectangle
    \param[in] p1 first point
    \param[in] p2 second point
 */
bool collides(const QRectF & r,const QPointF & p1, const QPointF & p2);


#endif // COLLISION_H
