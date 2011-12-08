#include <QStringList>
#include "saveload.h"

//saveload::saveload()
//{
//}

 QString  SaveLoad<int>::save(const int & object)
 {
  return QString::number(object);

 }
int SaveLoad<int>::load(const QString & string)
 {
   return string.toInt();
 }
QString SaveLoad<double>::save(const double & object)
{
   return QString::number(object);
}
 double SaveLoad<double>::load(const QString & string)
{
    bool ok=true;
    return string.toDouble(&ok);
}
 QString SaveLoad<DiagramParent >::save(const DiagramParent & loc)
{
 /*! ��������� int �� ������
          \param[in]  string ������ � ���������� �������
          \return  ��������������� ������
  */
     QString result=QString::number(loc.diagramID(),10);
     result+=";";
     result+=QString::number(loc.blockID(),10);
     return result;
}
 DiagramParent SaveLoad <DiagramParent >:: load(const QString & string)
 {
     QStringList lst=string.split(";");
     if (lst.size()!=2) return DiagramParent();
     bool ok=true;
     return DiagramParent(lst[0].toInt(&ok,10),lst[1].toInt(&ok,10));
 }

      QString SaveLoad <void *>:: save(void * ptr)
     {

         union
         {
             void * p;
            unsigned long    i;
         }  u;
         u.p=ptr;
         return QString::number(u.i,16);
     }
    void * SaveLoad <void *>::load(const QString & string)
    {
        union
        {
           void * p;
           unsigned long  i;
        }  u;
        bool ok=true;
        u.i=string.toInt(&ok,16);
        return u.p;
    }

 QString SaveLoad <QRect>::save(const QRect & r)
 {
     QString result=QString::number(r.x(),10)+QString(";")+
                    QString::number(r.y(),10)+QString(";")+
                    QString::number(r.width(),10)+QString(";")+
                    QString::number(r.height(),10);
     return result;
 }
  QRect SaveLoad <QRect>::load(const QString & string)
  {
      QStringList lst=string.split(";");
      if (lst.size()!=4) return QRect();
      bool ok=true;
      return QRect(lst[0].toInt(&ok,10),
                   lst[1].toInt(&ok,10),
                   lst[2].toInt(&ok,10),
                   lst[3].toInt(&ok,10));
  }
  QString SaveLoad <QRectF>::save(const QRectF & r)
  {
      QString result=QString::number(r.x())+QString(";")+
                     QString::number(r.y())+QString(";")+
                     QString::number(r.width())+QString(";")+
                     QString::number(r.height());
      return result;
  }
   QRectF SaveLoad <QRectF>::load( const QString & string)
   {

       QStringList lst=string.split(";");
       if (lst.size()!=4) return QRectF();
       bool ok=true;
       return QRectF(lst[0].toDouble(&ok),
                     lst[1].toDouble(&ok),
                     lst[2].toDouble(&ok),
                     lst[3].toDouble(&ok));
   }

    QString SaveLoad <QPointF>:: save( const QPointF & p)
    {
        QString result=QString::number(p.x());
        result+=";";
        result+=QString::number(p.y());
        return result;
    }
    QPointF  SaveLoad <QPointF>::load( const QString & string)
{
    QStringList lst=string.split(";");
    if (lst.size()!=2) return QPointF();
    bool ok=true;
    return QPointF(lst[0].toDouble(&ok),lst[1].toDouble(&ok));
}
//template<typename T1, typename T2>
//   QString SaveLoad <QPair <T1,T2> >:: save( const QPair  & p)
//   {
//       QString result;

//       return result;
//   }
//     QString  SaveLoad <QPair.T1, QPair.T2 >::save( const QPair  & p)
//     {
////         int t=10;
////         QString result=save(t) ;
////         return result;
//     }


 //inline QString locationToString( DiagramParent & loc)
 //{
 //    QString result=QString::number(loc.diagramID(),10);
 //    result+=";";
 //    result+=QString::number(loc.blockID(),10);
 //    return result;
 //}
