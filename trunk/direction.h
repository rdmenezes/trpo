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

#endif // DIRECTION_H
