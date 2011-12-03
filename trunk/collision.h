/*! \class  collision.h
    \author Mamontov
    Declares a collision testing functions
 */
#include <QRectF>
#include <QRect>
#include <QPoint>
#include <QPointF>
#include <QLineF>
#ifndef COLLISION_H
#define COLLISION_H

/*! Tests a collision between two rectangles
    \param[in] r1 first rectangle
    \param[in] r2 second rectangle
 */
bool collides(const QRectF & r1,const QRectF & r2);

/*! Tests a collisions between line and a rectangle
    \param[in] r rectangle
    \param[in] line line
 */
bool collides(const QRectF & r, const QLineF & line);

/*! Tests a collision between two lines
    \param[in] l1 first line
    \param[in] l2 second line
 */
bool collides(const QLineF & l1, const QLineF & l2);

#endif // COLLISION_H
