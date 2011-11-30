#include "collisionobject.h"

CollisionObject::~CollisionObject()
{
}

const QRectF & CollisionObject::collisionRect() const
{
    throw CollisionDataNotSpecified();
    return *(new QRectF());
}

const QLineF & CollisionObject::collisionLine() const
{
    throw CollisionDataNotSpecified();
    return *(new QLineF());
}

CollisionObject * CollisionObject::firstObject()
{
    throw CollisionDataNotSpecified();
    return NULL;
}

CollisionObject * CollisionObject::nextObject()
{
    throw CollisionDataNotSpecified();
    return NULL;
}
