#include "selecttool.h"

void ObjectChangingData::extractData(ObjectConnector * c)
{
 QVector< QPair<qreal,ObjectConnector*> > input=c->getConnected(C_INPUT);


}


void ObjectChangingData::extractData(ObjectConnector * c,
                                     Connection ct,
                                     const QVector< QPair<qreal,ObjectConnector *> > & from)
{

}
