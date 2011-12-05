/*! \file direction.h
    \author HiddenSeeker

    Declares a direction types for ObjectConnector
 */
#ifndef DIRECTION_H
#define DIRECTION_H

/*! Directions for ObjectConnector
 */
enum Direction
{
   D_LEFT=0,
   D_RIGHT=1,
   D_TOP=2,
   D_BOTTOM=3
};

/*! Is not collinear
    \param[in] input
    \param[in] self
    \return when is not collinear
 */
inline bool isNotCollinear(Direction input,Direction self)
{
   bool is_not_eq=input!=self;
   bool is_not_oph=!((input==D_LEFT && self==D_RIGHT) || (self==D_LEFT && input==D_RIGHT));
   bool is_not_opv=!((input==D_TOP && self==D_BOTTOM) || (self==D_BOTTOM && input==D_TOP));
   return is_not_eq && is_not_oph && is_not_opv;
}

/*! Tests, when directions are exactly opposite
    \param[in] d1 first direction
    \param[in] d2 second direction
 */
inline bool isOpposite(Direction dir1, Direction dir2)
{
    if (   (dir1==D_LEFT && dir2==D_RIGHT)
        || (dir2==D_LEFT && dir1==D_RIGHT) ) return true;
    if (   (dir1==D_TOP && dir2==D_BOTTOM)
        || (dir2==D_TOP && dir1==D_BOTTOM) ) return true;
    return false;
}

#endif // DIRECTION_H
