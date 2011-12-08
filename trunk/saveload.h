#ifndef SAVELOAD_H
#define SAVELOAD_H

//class saveload
//{
//public:
//    saveload();
//};
#include <QString>
#include <QStringList>
#include <QRect>
#include <QRectF>
#include <QPointF>
#include <QPair>
#include "diagram.h"


//Общий класс, создающий единство интерфейса для сериализации и десериализации примитивов
template<typename T >
class SaveLoad
{
  /*! Сохраняет объект в строку
           \param[in] object объект для сохранения
           \return строка с содержимым объекта
   */
  static QString save(const T & object)
  {
      QString result=s;

      return result;
  }
  /*! Загружает объект из строки
           \param[in]  string строка с содержимым объекта
           \return  восстановленный объект
   */
  static T load(const QString & string)
  {
     T result=string;
      /*
      for (int i=escape_len-1;i>-1;i--)
          result.replace(escape_to[i],escape_from[i]);
      */
      return T;

  }
};

//Это инстанцирование шаблона определяет поведение для типа int
template<>
class SaveLoad<int>
{
  /*! Сохраняет int в строку
           \param[in] object объект для сохранения
           \return строка с содержимым объекта
   */
  static QString save(const int & object);
//  {
//   return QString::number(object);
//  }
  /*! Загружает int из строки
           \param[in]  string строка с содержимым объекта
           \return  восстановленный объект
   */
   static int load(const QString & string);
//   {
//     return string.toInt();
//   }
};



template<>
class SaveLoad<double>
{
    static QString save(const double & object);

    /*! Загружает int из строки
             \param[in]  string строка с содержимым объекта
             \return  восстановленный объект
     */

     static double load(const QString & string);


/*! Converts string to double
 */

};

template<>
class SaveLoad<DiagramParent>
{
    static QString save(const DiagramParent & loc);

    /*! Загружает int из строки
             \param[in]  string строка с содержимым объекта
             \return  восстановленный объект
     */

     static DiagramParent load(const QString & string);


/*! Converts string to double
 */

};
template<>
class SaveLoad <void *>

{
    static QString save( void * ptr);
   static void * load(const QString & string);
};
template<>
class SaveLoad <QRect>
{


   static QString save(const QRect & r);
  static QRect load(const QString & string);
};
template<>
class SaveLoad <QRectF>
{


   static QString save(const QRectF & r);
  static QRectF load(const QString & string);
};



template<>
class SaveLoad <QPointF>
{
   static QString save( const QPointF & p);
   static QPointF load( const QString & string);
};
template<typename T1, typename T2>
class SaveLoad <QPair <T1,T2> >
{
   static QString save( const QPair  & p)
   {

               QString result1=save(p.first) ;
               QString result2=save(p.second) ;
               QString result;
               result.append(result1,"@",result2);
               return result;
   }
   static QPair load( const QString & string)
   {
       QPair tmpPair;


        QString str1 =   string.section('@', 0, 0);
         QString str2 =   string.section('@', 1, 1);

tmpPair.first=str1;
tmpPair.second=str2;
 return tmpPair;




   }
};


//inline QString locationToString( DiagramParent & loc)
//{
//    QString result=QString::number(loc.diagramID(),10);
//    result+=";";
//    result+=QString::number(loc.blockID(),10);
//    return result;
//}

#endif // SAVELOAD_H
