/*! \file   saveloadutils.h
    \author Mamontov
    Describes utility functions used to parse data
 */
#ifndef SAVELOADUTILS_H
#define SAVELOADUTILS_H

#include <QString>
#include <QPointF>
#include <QRect>
#include <QRectF>
#include "diagram.h"
#include <QStringList>

/*! Converts int to string
 */
inline QString intToString(int a)
{
    return QString::number(a,10);
}
/*! Converts string to int
 */
inline int stringToInt(const QString & s)
{
    bool ok=true;
    return s.toInt(&ok,10);
}
/*! Converts double to string
 */
inline QString doubleToString(double a)
{
    return QString::number(a);
}
/*! Converts string to double
 */
inline double stringToDouble(const QString & s)
{
    bool ok=true;
    return s.toDouble(&ok);
}
/*! Converts parent location to string
 */
inline QString locationToString( ParentLocation & loc)
{
    QString result=QString::number(loc.diagramID(),10);
    result+=";";
    result+=QString::number(loc.blockID(),10);
    return result;
}

/*! Converts parent location to string
 */
inline ParentLocation stringToLocation( const QString & s)
{
    QStringList lst=s.split(";");
    if (lst.size()!=2) return ParentLocation();
    bool ok=true;
    return ParentLocation(lst[0].toInt(&ok,10),lst[1].toInt(&ok,10));
}

/*! Converts a pointer to string
 */
inline QString ptrToString(void * ptr)
{
    union
    {
        void * p;
        int    i;
    }  u;
    u.p=ptr;
    return QString::number(u.i,16);
}

/*! Converts a string to pointer
 */
inline void * stringToPtr(const QString & s)
{
    union
    {
        void * p;
        int    i;
    }  u;
    bool ok=true;
    u.i=s.toInt(&ok,16);
    return u.p;
}
/*! Preprocesses a string for addition to XML
 */
QString stringToStringXML(const QString & s);
/*! Preprocesses a string for addition to XML
 */
QString stringXMLToString(const QString & s);

/*! Converts a qrect to a string
 */
QString rectToString(const QRect & r);
/*! Converts a qrect to a string
 */
QRect stringToRect(const QString & s);

/*! Converts a qrect to a string
 */
QString rectfToString(const QRectF & r);
/*! Converts a qrect to a string
 */
QRectF stringToRectf(const QString & s);

/*! Converts a qrect to a string
 */
QString pointfToString(const QPointF & p);
/*! Converts a qrect to a string
 */
QPointF stringToPointf(const QString & s);

#endif // SAVELOADUTILS_H
