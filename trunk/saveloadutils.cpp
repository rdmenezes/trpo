#include "saveloadutils.h"
/*
static int escape_len=6;
static const char * escape_from[6]={
 "&",
 "\'",
 "\"",
 "<",
 ">",
 "\n"
};
static const char * escape_to[6]={
 "&amp;",
 "&apos;",
 "&quote;",
 "&lt;",
 "&gt;",
 "&#10;"
};
*/
QString stringToStringXML(const QString & s)
{
  QString result=s;
  /*
  for (int i=0;i<escape_len;i++)
      result.replace(escape_from[i],escape_to[i]);
  */
  return result;
}

QString stringXMLToString(const QString & s)
{
 QString result=s;
 /*
 for (int i=escape_len-1;i>-1;i--)
     result.replace(escape_to[i],escape_from[i]);
 */
 return result;
}

QString rectToString(const QRect & r)
{
  QString result=QString::number(r.x(),10)+QString(";")+
                 QString::number(r.y(),10)+QString(";")+
                 QString::number(r.width(),10)+QString(";")+
                 QString::number(r.height(),10);
  return result;
}

QRect stringToRect(const QString & s)
{
  QStringList lst=s.split(";");
  if (lst.size()!=4) return QRect();
  bool ok=true;
  return QRect(lst[0].toInt(&ok,10),
               lst[1].toInt(&ok,10),
               lst[2].toInt(&ok,10),
               lst[3].toInt(&ok,10));

}


QString rectfToString(const QRectF & r)
{
  QString result=QString::number(r.x())+QString(";")+
                 QString::number(r.y())+QString(";")+
                 QString::number(r.width())+QString(";")+
                 QString::number(r.height());
  return result;
}

QRectF stringToRectf(const QString & s)
{
  QStringList lst=s.split(";");
  if (lst.size()!=4) return QRectF();
  bool ok=true;
  return QRectF(lst[0].toDouble(&ok),
                lst[1].toDouble(&ok),
                lst[2].toDouble(&ok),
                lst[3].toDouble(&ok));
}


/*! Converts parent location to string
 */
QString pointfToString( const QPointF & p)
{
    QString result=QString::number(p.x());
    result+=";";
    result+=QString::number(p.y());
    return result;
}

/*! Converts parent location to string
 */
QPointF stringToPointf( const QString & s)
{
    QStringList lst=s.split(";");
    if (lst.size()!=2) return QPointF();
    bool ok=true;
    return QPointF(lst[0].toDouble(&ok),lst[1].toDouble(&ok));
}
