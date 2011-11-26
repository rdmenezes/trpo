#include "collisionobject.h"

CollisionObject::~CollisionObject()
{
}

ShapeType RectangularCollisionObject::collisionShape() const     { return ST_RECTANGLE; }
ShapeType LinearCollisionObject     ::collisionShape() const     { return ST_LINE; }
ShapeType ComplexCollisionObject    ::collisionShape() const     { return ST_COMPLEX; }

