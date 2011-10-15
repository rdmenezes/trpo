/*! \file boxitem.h
    \author HiddenSeeker

    Declares a box graphics item, that represents block
 */
#ifndef BOXITEM_H
#define BOXITEM_H

#include <QGraphicsItem>
//Class of scene
class DiagramScene;
/*! \class BoxItem

    Declares a box item class
 */
class BoxItem : public QGraphicsItem
{
public:
    enum ItemType
    {
        USERTYPE=QGraphicsItem::UserType+1
    };

    BoxItem();
};

#endif // BOXITEM_H
