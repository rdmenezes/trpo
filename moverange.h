/*! \file   moverange.h
    \author Mamontov

    Declares a moving range policy
 */
#ifndef MOVERANGE_H
#define MOVERANGE_H

#include <QRectF>

/*! \class MoveRange
    Declares a moving range policy data
 */
class MoveRange : public QRectF
{
public:
    /*! Parametric data to move to
        \param[in] x x
        \param[in] y y data
        \param[in] w width
        \param[in] h height
     */
    MoveRange(qreal x, qreal y,qreal w, qreal h);
    /*! Tests whether point is within range
        \param[in] p point
     */
    bool isWithin(const QPointF & p);
    /*! Reduces a range by other range
     */
    void reduce(const MoveRange & range);
};

/*!  Constructs a horizontal range
     \param[in] x1 minimal range
     \param[in] x2 maximal range
     \param[in] y  y
 */
MoveRange createHorizontalRange(qreal x1, qreal x2, qreal y);
/*!  Constructs a vertical range
     \param[in] y1 minimal range
     \param[in] y2 maximal range
     \param[in] x  x
 */
MoveRange createVerticalRange(qreal y1, qreal y2, qreal x);


#endif // MOVERANGE_H
