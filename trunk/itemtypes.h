/*! \file itemtypes.h
    \author Mamontov
    Describes known item types
 */
#ifndef ITEMTYPES_H
#define ITEMTYPES_H
#include <QGraphicsItem>

/*! Describes am item type
 */
enum ItemTypes
{
    IsDefaultItem=QGraphicsItem::UserType+1,
    IsBox        =QGraphicsItem::UserType+2,
    IsArrow      =QGraphicsItem::UserType+3,
    IsFreeComment=QGraphicsItem::UserType+4,
    IsCommentLine=QGraphicsItem::UserType+5
};

#endif // ITEMTYPES_H
